import random

# ARM64 32-bit words
# 010000D4 -> svc #0
# C0035FD6 -> return_type_arg
# LE byte arrays
original_bytes = [
    0xD4, 0x00, 0x00, 0x01,  # 010000D4
    0xD6, 0x5F, 0x03, 0xC0   # C0035FD6
]

key = [random.randint(0, 255) for _ in range(8)]

encrypted_stub = [b ^ k for b, k in zip(original_bytes, key)]

print("static const uint8_t secure_encrypted_stub[8] = {")
print("    " + ", ".join(f"0x{b:02X}" for b in encrypted_stub))
print("};")
print("static const uint8_t secure_key[8] = {")
print("    " + ", ".join(f"0x{k:02X}" for k in key))
print("};")