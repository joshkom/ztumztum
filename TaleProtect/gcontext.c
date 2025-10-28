#include "gcontext.h"
#include "global_common/common.h"
#include "TaleProtect/symbol_list/symbol_list.h"
#include "TaleProtect/indirect_registration/indirect_registration.h"
#include <unstdmemory_compat.h>

TaleProtectContext_t TaleProtectContext = {
        {0},
        .not_an_elf.keys = {
                .key = {
                        0xfa, 0xba, 0x01, 0xf2, 0x84, 0x19, 0xf5, 0xf1,
                        0xff, 0xbb, 0x3f, 0xaf, 0x72, 0x39, 0xfe, 0xdc,
                        0xf8, 0xb1, 0xa0, 0xa3, 0xfe, 0x67, 0xf1, 0xd4,
                        0xff, 0xb1, 0x3f, 0xc7, 0x91, 0x60, 0xfc, 0x3f
                }, .iv = {
                        0x94, 0x44, 0x49, 0xcb, 0xa3, 0x1c, 0xe6, 0xee,
                        0x11, 0x44, 0x44, 0xc1, 0x12, 0x13, 0xfb, 0x42
                }
        }
};

void TaleProtectContextInit(void) {
    u8 *package_name_15 = decode_com_tale_royale__15();
    LOGI("(TaleProtectContextInit) package_name: %s", package_name_15);
    unstdmemory_compat_memcpy(TaleProtectContext.package_name, package_name_15, 15);
    freeM(package_name_15);

    putex_init(&TaleProtectContext.process_monitor.maps_monitor.maps_entries_protection.putex, 0);

    indirect_registration_ctx_init();
}
