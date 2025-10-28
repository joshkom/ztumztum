#include "list_dir_entries.h"

#include <linux/fcntl.h>
#include <dirent.h>

#include <unstdstring_compat.h>

#include "TaleProtect/secure_syscall/secure_syscall.h"
#include "TaleProtect/tools/utils.h"

#define BUF_SIZE 4096

struct linux_dirent64 {
    uint64_t d_ino;
    int64_t d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char d_name[];
};

list_dir_entries_stacked_result_t list_dir_entries_stacked(const i8 *path) {
    list_dir_entries_stacked_result_t result = {0};
    i8 buf[BUF_SIZE];

    i32 fd = (i32) secure_syscall_v2M(
            SYS_openat,
            4,
            AT_FDCWD,
            path,
            O_RDONLY | O_DIRECTORY,
            0
    );

    if (fd < 0) {
        LOGW("(Tools::ListDirEntries) openat failed on %s: %s", path, strerror(errno));
        return result;
    }

    ssizet nread = secure_syscall_v2M(
            SYS_getdents64,
            3,
            fd,
            buf,
            BUF_SIZE
    );

    if (nread <= 0) {
        LOGW("(Tools::ListDirEntries) getdents64 failed or empty for %s: %s", path,
             strerror(errno));
        secure_syscall_v2M(SYS_close, 1, fd);
        return result;
    }

    sizet bpos = 0;
    while (bpos < (sizet) nread && result.count < 256) {
        struct linux_dirent64 *d = (struct linux_dirent64 *) (buf + bpos);
        bpos += d->d_reclen;

        if (d->d_name[0] == '.'
            && (d->d_name[1] == '\0'
                || (d->d_name[1] == '.'
                    && d->d_name[2] == '\0'))) {
            continue;
        }

        list_dir_entries_entry_t *entry = &result.entries[result.count++];
        unstdmemory_compat_memset(entry->name, 0, sizeof(entry->name));
        unstdmemory_compat_memcpy(entry->name, d->d_name, unstdstring_compat_strlen8(d->d_name));
        entry->type = d->d_type;

        const char *type_str =
                (d->d_type == DT_REG) ? "file" :
                (d->d_type == DT_DIR) ? "dir" :
                (d->d_type == DT_LNK) ? "link" :
                (d->d_type == DT_SOCK) ? "sock" :
                (d->d_type == DT_FIFO) ? "fifo" :
                (d->d_type == DT_CHR) ? "char" :
                (d->d_type == DT_BLK) ? "block" : "other";

        LOGD("(Tools::ListDirEntries) entry: %s (%s)", d->d_name, type_str);
    }

    secure_syscall_v2M(SYS_close, 1, fd);
    LOGI("(Tools::ListDirEntries) total entries in %s: %u", path, result.count);
    return result;
}


list_dir_entries_alloced_result_t list_dir_entries_alloced(const i8 *path) {
    list_dir_entries_alloced_result_t result = {0};
    result.entries = NULL;
    result.count = 0;
    result.capacity = 0;

    i8 buf[BUF_SIZE];

    i32 fd = (i32) secure_syscall_v2M(SYS_openat, 4, AT_FDCWD, path, O_RDONLY | O_DIRECTORY, 0);
    if (fd < 0) {
        LOGW("(Tools::ListDirEntriesAlloced) openat failed on %s: %s", path, strerror(errno));
        return result;
    }

    ssizet nread = secure_syscall_v2M(SYS_getdents64, 3, fd, buf, BUF_SIZE);
    if (nread <= 0) {
        LOGW("(Tools::ListDirEntriesAlloced) getdents64 failed or empty for %s: %s", path,
             strerror(errno));
        secure_syscall_v2M(SYS_close, 1, fd);
        return result;
    }

    sizet bpos = 0;
    while (bpos < (sizet) nread) {
        struct linux_dirent64 *d = (struct linux_dirent64 *) (buf + bpos);
        bpos += d->d_reclen;

        if (d->d_name[0] == '.'
            && (d->d_name[1] == '\0'
                || (d->d_name[1] == '.'
                    && d->d_name[2] == '\0'))) {
            continue;
        }

        if (result.count >= result.capacity) {
            u32t new_cap = (result.capacity == 0) ? 64 : result.capacity * 2;
            void *new_ptr = reallocM(
                    result.entries,
                    new_cap * sizeof(list_dir_entries_entry_t)
            );

            if (!new_ptr) {
                LOGE("(Tools::ListDirEntriesAlloced) realloc failed (capacity=%u)", new_cap);
                break;
            }
            result.entries = new_ptr;
            result.capacity = new_cap;
        }

        list_dir_entries_entry_t *entry = &result.entries[result.count++];
        unstdmemory_compat_memset(entry->name, 0, sizeof(entry->name));
        unstdmemory_compat_memcpy(entry->name, d->d_name, unstdstring_compat_strlen8(d->d_name));
        entry->type = d->d_type;

        const char *type_str =
                (d->d_type == DT_REG) ? "file" :
                (d->d_type == DT_DIR) ? "dir" :
                (d->d_type == DT_LNK) ? "link" :
                (d->d_type == DT_SOCK) ? "sock" :
                (d->d_type == DT_FIFO) ? "fifo" :
                (d->d_type == DT_CHR) ? "char" :
                (d->d_type == DT_BLK) ? "block" : "other";

        LOGD("(Tools::ListDirEntriesAlloced) entry: %s (%s)", d->d_name, type_str);
    }

    secure_syscall_v2M(SYS_close, 1, fd);
    LOGI("(Tools::ListDirEntriesAlloced) total entries in %s: %u", path, result.count);
    return result;
}

void list_dir_entries_free(list_dir_entries_alloced_result_t *buffer_arg) {
    freeM(buffer_arg->entries);
}
