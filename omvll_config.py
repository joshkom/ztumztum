import omvll
from functools import lru_cache

class MyConfig(omvll.ObfuscationConfig):
    def __init__(self):
        super().__init__()

    def break_control_flow(self, mod: omvll.Module, resolved_ptr: omvll.Function):
        substr_keywords = ["curl"]
        exact_keywords = ["JNIOnload", "JNI_OnLoad"]

        name_lower = resolved_ptr.demangled_name.lower()

        # Check exact matches (case-insensitive)
        if any(name_lower == kw.lower() for kw in exact_keywords):
            print("EXACTmatch:", name_lower)
            return omvll.BreakControlFlowOpt(True)

        # Check substring matches (case-insensitive)
        if any(kw in name_lower for kw in substr_keywords):
            print("SUBmatch:", name_lower)
            return omvll.BreakControlFlowOpt(True)

        # return omvll.BreakControlFlowOpt(True)

    def anti_hooking(self, mod: omvll.Module, resolved_ptr: omvll.Function) -> omvll.AntiHookOpt:
        if resolved_ptr.name in ["encrypt", "svc", "obf", "tool"]:
            return True
        return False

#
# #     def break_control_flow(self, mod: omvll.Module, fun: omvll.Function):
# #         return True  # Enable break control flow for all
#
#     def break_control_flow(self, mod: omvll.Module, resolved_ptr: omvll.Function):
#         return True

    def flatten_cfg(self, mod: omvll.Module, fun: omvll.Function):
        return True  # Flatten all functions

    def obfuscate_arithmetic(self, mod: omvll.Module, fun: omvll.Function):
        return True  # Obfuscate all arithmetic ops

    def obfuscate_string(self, mod: omvll.Module, fun: omvll.Function, string: bytes):
        # return omvll.StringEncOptGlobal()
        return omvll.StringEncOptGlobal()

    # def indirect_branch(self, mod: omvll.Module, resolved_ptr: omvll.Function):
    #     return True
    #     # return omvll.ObfuscationConfig.default_config(self, mod, resolved_ptr, ["third-party/"], [], [], 5)

    def indirect_call(self, mod: omvll.Module, resolved_ptr: omvll.Function):
        return True

    def obfuscate_constants(_, __, resolved_ptr: omvll.Function):
        return [0xdeadb00bbadbabe]

        # if "init_context_all" in resolved_ptr.demangled_name:
        #     # Only obfuscate the constants that are in this list
        #     return [0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19]
        # return False
    # Skip obfuscating third-party modules and apply the pass with a 20% likelihood.
    #     return omvll.ObfuscationConfig.default_config(self, mod, fun, ["third-party/"], [], [], 20)

    def obfuscate_struct_access(self, _: omvll.Module, __: omvll.Function, struct: omvll.Struct):
        if struct.name.endswith("JNINativeInterface"):
            return True
        if struct.name.endswith("JNI"):
            return True
        if "jni" in struct.name.lower():
            return True
        return False

@lru_cache(maxsize=1)
def omvll_get_config() -> omvll.ObfuscationConfig:
    #! todo: DONT FORGET TO UNCOMMENT THIS!!!!
    omvll.config.shuffle_functions = True
    omvll.config.inline_jni_wrappers = True
    return MyConfig()
