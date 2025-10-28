#include "code_integrity_gadget_royale_trusted_conditional.h"
#include "code_integrity_gadget_royale_trusted.h"
#include <TaleProtect/obfuscators/literal_obfuscator.h>
#include "TaleProtect/demolisher/rainbow_terminator.h"

code_integrity_gadget_royale_trusted_conditional_ctx_t code_integrity_gadget_royale_trusted_conditional_ctx = {
        0
};

void dont_lie_to_me_senpaikun_compute_memory_range(void *base, segment_t *seg) {
    if (!base || !seg) return;

    u8t *mem = (u8t *) base;
    const u8t *e_ident = mem;

    if (memcmp(e_ident, ELFMAG, SELFMAG) != 0) { return; }

    u8t *seg_start = (u8t *) base;
    u8t *seg_end = (u8t *) base;

    if (e_ident[EI_CLASS] == ELFCLASS64) {
        Elf64_Ehdr *ehdr = (Elf64_Ehdr *) mem;
        Elf64_Phdr *phdr = (Elf64_Phdr *) (mem + ehdr->e_phoff);

        for (Elf64_Half i = 0; i < ehdr->e_phnum; i++) {
            if (phdr[i].p_type != PT_LOAD) { continue; }
            u8t *start_addr = (u8t *) ((uptrt) base + phdr[i].p_vaddr);
            u8t *end_addr = start_addr + phdr[i].p_memsz;
            if (start_addr < seg_start) seg_start = start_addr;
            if (end_addr > seg_end) seg_end = end_addr;
        }
    } else if (e_ident[EI_CLASS] == ELFCLASS32) {
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *) mem;
        Elf32_Phdr *phdr = (Elf32_Phdr *) (mem + ehdr->e_phoff);

        for (Elf32_Half i = 0; i < ehdr->e_phnum; i++) {
            if (phdr[i].p_type != PT_LOAD) { continue; }
            u8t *start_addr = (u8t *) ((uptrt) base + phdr[i].p_vaddr);
            u8t *end_addr = start_addr + phdr[i].p_memsz;
            if (start_addr < seg_start) seg_start = start_addr;
            if (end_addr > seg_end) seg_end = end_addr;
        }
    }

    seg->start = seg_start;
    seg->size = seg_end - seg_start;
}

void dont_lie_to_me_senpaikun(void *void_0_arg,
                              void *void_1_arg,
                              u32t its_called_motion_arg,
                              void *void_3_arg,
                              void *void_4_arg,
                              void *void_5_arg,
                              void *libtale_pc_arg,
                              void *libroyale_pc_arg) {
    LOGD("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::Conditional::dont_lie_to_me_senpaikun) called with: motion: 0x%x | tb: %p | sb: %p",
         its_called_motion_arg, libtale_pc_arg, libroyale_pc_arg
    );

    u32t motion = OBF_INT32(0xfaaebabe);
    if (its_called_motion_arg != (u32t) DEOBF_INT32(motion)) {
        LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::Conditional::dont_lie_to_me_senpaikun) invalid motion");
        demolisher_rainbow_terminator();
    }

    LOGW("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::Conditional::dont_lie_to_me_senpaikun) calculating bases...");

    segment_t *tale_seg = &TaleProtectContext.integrity_monitor
            .code_integrity.external.gadgets.whitelist_ranges.tale;
    segment_t *royale_seg = &TaleProtectContext.integrity_monitor
            .code_integrity.external.gadgets.whitelist_ranges.royale;

    tale_seg->start = NULL;
    tale_seg->size = 0;
    royale_seg->start = NULL;
    royale_seg->size = 0;

    Dl_info info;

    if (dladdrM(libtale_pc_arg, &info) && info.dli_fbase) {
        dont_lie_to_me_senpaikun_compute_memory_range(info.dli_fbase, tale_seg);
    }

    if (dladdrM(libroyale_pc_arg, &info) && info.dli_fbase) {
        dont_lie_to_me_senpaikun_compute_memory_range(info.dli_fbase, royale_seg);
    }

    LOGW("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::Conditional::dont_lie_to_me_senpaikun) calculation finished.");

    LOGD("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::Conditional::dont_lie_to_me_senpaikun) [TB] %p+0x%zx | [SB]: %p+0x%zx",
         TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.whitelist_ranges.tale.start,
         TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.whitelist_ranges.tale.size,
         TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.whitelist_ranges.royale.start,
         TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.whitelist_ranges.royale.size
    );

//    putex_signal(&code_integrity_gadget_royale_trusted_conditional_ctx.royale_hashchecker_invoker);
    shemaphore_signal(&code_integrity_gadget_royale_trusted_conditional_ctx.royale_hashchecker_sem);
}

void *code_integrity_gadget_royale_trusted_conditional_runner(void *arg) {
    (void) arg;

    if (shemaphore_wait_timeout(
            &code_integrity_gadget_royale_trusted_conditional_ctx.royale_hashchecker_sem,
            12, 0) != SHEMAPHORE_ACQUIRED) {
        LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::Conditional::Runner) semaphore timeout");
        demolisher_smash();
        return NULL;
    }

    LOGD("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::Conditional::Runner) semaphore signal received.");
    code_integrity_gadget_royale_trusted();

    return NULL;
}

void code_integrity_gadget_royale_trusted_conditional(void) {
    shemaphore_init(&code_integrity_gadget_royale_trusted_conditional_ctx.royale_hashchecker_sem, 0);

    pthread_t runner_tid;
    if (pthread_createM(&runner_tid, NULL,
                        code_integrity_gadget_royale_trusted_conditional_runner, NULL)) {
        demolisher_smash();
        demolisher_stackoverflower();
    }
}