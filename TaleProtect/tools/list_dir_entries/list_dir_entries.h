#ifndef TALESTARS_V63_322_LIST_DIR_ENTRIES_H
#define TALESTARS_V63_322_LIST_DIR_ENTRIES_H

#include <unstdinttypes.h>

typedef enum list_dir_entries_type_e : u8t {
    LDE_ENTRY_TYPE_UNKNOWN = 0,
/** d_type value for a FIFO. */
    LDE_ENTRY_TYPE_FIFO = 1,
/** d_type value for a character device. */
    LDE_ENTRY_TYPE_CHRDEV = 2,
/** d_type value for a directory. */
    LDE_ENTRY_TYPE_DIR = 4,
/** d_type value for a block device. */
    LDE_ENTRY_TYPE_BLKDEV = 6,
/** d_type value for a regular file. */
    LDE_ENTRY_TYPE_FILE = 8,
/** d_type value for a symbolic link. */
    LDE_ENTRY_TYPE_SYMLINK = 10,
/** d_type value for a socket. */
    LDE_ENTRY_TYPE_SOCK = 12,
    LDE_ENTRY_TYPE_WHT = 14
} list_dir_entries_type_t;

typedef struct list_dir_entries_entry_s {
    i8 name[256];
    list_dir_entries_type_t type;
} list_dir_entries_entry_t;

//! [stack-based]
typedef struct list_dir_entries_result_s {
    list_dir_entries_entry_t entries[256];
    u32t count;
} list_dir_entries_stacked_result_t;

list_dir_entries_stacked_result_t list_dir_entries_stacked(
        const i8 *path
);

//! [heap-based]
typedef struct list_dir_entries_alloced_result_s {
    list_dir_entries_entry_t *entries;
    u32t count;
    u32t capacity;
} list_dir_entries_alloced_result_t;

list_dir_entries_alloced_result_t list_dir_entries_alloced(
        const i8 *path
);

void list_dir_entries_free(
        list_dir_entries_alloced_result_t *buffer_arg
);

#endif //TALESTARS_V63_322_LIST_DIR_ENTRIES_H
