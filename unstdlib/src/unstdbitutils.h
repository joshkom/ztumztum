#ifndef UNSTDLIB_UNSTDBITUTILS_H
#define UNSTDLIB_UNSTDBITUTILS_H

#include "unstdinttypes.h"
#include "unstdbool.h"

#include <stdarg.h>
#include <stdio.h>

//! [bit manipulation utilities]
/**
 * @brief Checks if a value_arg fits within a specified number of bits_arg in an maximum 32-bit range.
 * @param value_arg The value to check.
 * @param bits_arg The number of bits to fit the `value_arg` into.
 * @returns True if `value_arg` fits within `bits_arg` bits_arg; otherwise, False.
 * @retval [true] The value_arg fits within the specified number of bits_arg.
 * @retval [false] The value_arg does not fit within the specified number of bits_arg.
 */
extern bool unstdbitutils_fitsinbits(
    u32t value_arg,
    u8t bits_arg
);

/**
 * @brief Packs multiple (bits, value) pairs into an 8-bit byte, shifting left-to-right (LTR).
 * @param pairs_count_arg Number of (bits, value) pairs to be packed.
 * @param out_error_arg Should be a pointer to a u8t where the error code will be stored. Pass NULL to ignore errors.
 * @param ... Variable argument list of (bits, value) pairs.
 *             - `bits` specifies the number of bits used by `value`.
 *             - `value` is the value to be packed, limited to `bits` bits.
 * @returns The packed 8-bit byte.
 * @OutParam <strong>out_error_arg</strong> A number (u8t) indicating the state of the operation.
 * @OutParamValue [1] Success. The byte was packed successfully.
 * @OutParamValue [2] Failure. The combined bits exceed the 8-bit boundary.
 * @OutParamValue [3] Failure. A value does not fit in the specified number of bits.
 */
extern u8t unstdbitutils_packltrmulti8(
    u8t pairs_count_arg,
    u8t *const out_error_arg,
    ...
);

//! [bit manipulation utilities]
/**
 * @brief Packs multiple (bits, value) pairs into an 8-bit byte, shifting right-to-left (RTL).
 * @param pairs_count_arg Number of (bits, value) pairs to be packed.
 * @param out_error_arg Should be a pointer to a u8t where the error code will be stored. Pass NULL to ignore errors.
 * @param ... Variable argument list of (bits, value) pairs.
 *             - `bits` specifies the number of bits used by `value`.
 *             - `value` is the value to be packed, limited to `bits` bits.
 * @returns The packed 8-bit byte.
 * @OutParam <strong>out_error_arg</strong> A number (u8t) indicating the state of the operation.
 * @OutParamValue [1] Success. The byte was packed successfully.
 * @OutParamValue [2] Failure. The combined bits exceed the 8-bit boundary.
 * @OutParamValue [3] Failure. A value does not fit in the specified number of bits.
 */
extern u8t unstdbitutils_packrtlmulti8(
    u8t pairs_count_arg,
    u8t *const out_error_arg,
    ...
);

/**
 * @brief Unpacks an 8-bit byte into multiple values based on the (bits, value) pairs, shifting left-to-right (LTR).
 * @param packet_buffer_arg The 8-bit byte to unpack.
 * @param pairs_count_arg Number of (bits, value) pairs to be unpacked.
 * @param out_error_arg Should be a pointer to a u8t where the error code will be stored. Pass NULL to ignore errors.
 * @param ... Variable argument list of (bits, value) pairs.
 *             - `bits` specifies the number of bits used by each value.
 *             - `value` is a pointer to store the extracted value. Pass NULL if the value should be skipped.
 * @returns None.
 * @OutParam <strong>out_error_arg</strong> A number (u8t) indicating the state of the operation.
 * @OutParamValue [1] Success. The byte was unpacked successfully.
 * @OutParamValue [2] Failure. The combined bits exceed the 8-bit boundary.
 */
extern void unstdbitutils_unpackltrmulti8(
    u8t packet_buffer_arg,
    u8t pairs_count_arg,
    u8t *const out_error_arg,
    ...
);

/**
 * @brief Unpacks an 8-bit byte into multiple values based on the (bits, value) pairs, shifting right-to-left (RTL).
 * @param packet_buffer_arg The 8-bit byte to unpack.
 * @param pairs_count_arg Number of (bits, value) pairs to be unpacked.
 * @param out_error_arg Should be a pointer to a u8t where the error code will be stored. Pass NULL to ignore errors.
 * @param ... Variable argument list of (bits, value) pairs.
 *             - `bits` specifies the number of bits used by each value.
 *             - `value` is a pointer to store the extracted value. Pass NULL if the value should be skipped.
 * @returns None.
 * @OutParam <strong>out_error_arg</strong> A number (u8t) indicating the state of the operation.
 * @OutParamValue [1] Success. The byte was unpacked successfully.
 * @OutParamValue [2] Failure. The combined bits exceed the 8-bit boundary.
 */
extern void unstdbitutils_unpackrtlmulti8(
    u8t packet_buffer_arg,
    u8t pairs_count_arg,
    u8t *const out_error_arg,
    ...
);

//! [16-bit]

/**
 * @brief Packs multiple (bits, value) pairs into a 16-bit value, shifting left-to-right (LTR).
 * @param pairs_count_arg Number of (bits, value) pairs to be packed.
 * @param out_error_arg Should be a pointer to a u8t where the error code will be stored. Pass NULL to ignore errors.
 * @param ... Variable argument list of (bits, value) pairs.
 *             - `bits` specifies the number of bits used by `value`.
 *             - `value` is the value to be packed, limited to `bits` bits.
 * @returns The packed 16-bit value.
 * @OutParam <strong>out_error_arg</strong> A number (u8t) indicating the state of the operation.
 * @OutParamValue [1] Success. The value was packed successfully.
 * @OutParamValue [2] Failure. The combined bits exceed the 16-bit boundary.
 * @OutParamValue [3] Failure. A value does not fit in the specified number of bits.
 */
extern u16t unstdbitutils_packltrmulti16(
    u8t pairs_count_arg,
    u8t *const out_error_arg,
    ...
);

/**
 * @brief Packs multiple (bits, value) pairs into a 16-bit value, shifting right-to-left (RTL).
 * @param pairs_count_arg Number of (bits, value) pairs to be packed.
 * @param out_error_arg Should be a pointer to a u8t where the error code will be stored. Pass NULL to ignore errors.
 * @param ... Variable argument list of (bits, value) pairs.
 *             - `bits` specifies the number of bits used by `value`.
 *             - `value` is the value to be packed, limited to `bits` bits.
 * @returns The packed 16-bit value.
 * @OutParam <strong>out_error_arg</strong> A number (u8t) indicating the state of the operation.
 * @OutParamValue [1] Success. The value was packed successfully.
 * @OutParamValue [2] Failure. The combined bits exceed the 16-bit boundary.
 * @OutParamValue [3] Failure. A value does not fit in the specified number of bits.
 */
extern u16t unstdbitutils_packrtlmulti16(
    u8t pairs_count_arg,
    u8t *const out_error_arg,
    ...
);

/**
 * @brief Unpacks a 16-bit value into multiple values based on the (bits, value) pairs, shifting left-to-right (LTR).
 * @param packet_buffer_arg The 16-bit value to unpack.
 * @param pairs_count_arg Number of (bits, value) pairs to be unpacked.
 * @param out_error_arg Should be a pointer to a u8t where the error code will be stored. Pass NULL to ignore errors.
 * @param ... Variable argument list of (bits, value) pairs.
 *             - `bits` specifies the number of bits used by each value.
 *             - `value` is a pointer to store the extracted value. Pass NULL if the value should be skipped.
 * @returns None.
 * @OutParam <strong>out_error_arg</strong> A number (u8t) indicating the state of the operation.
 * @OutParamValue [1] Success. The value was unpacked successfully.
 * @OutParamValue [2] Failure. The combined bits exceed the 16-bit boundary.
 */
extern void unstdbitutils_unpackltrmulti16(
    u16t packet_buffer_arg,
    u8t pairs_count_arg,
    u8t *const out_error_arg,
    ...
);

/**
 * @brief Unpacks a 16-bit value into multiple values based on the (bits, value) pairs, shifting right-to-left (RTL).
 * @param packet_buffer_arg The 16-bit value to unpack.
 * @param pairs_count_arg Number of (bits, value) pairs to be unpacked.
 * @param out_error_arg Should be a pointer to a u8t where the error code will be stored. Pass NULL to ignore errors.
 * @param ... Variable argument list of (bits, value) pairs.
 *             - `bits` specifies the number of bits used by each value.
 *             - `value` is a pointer to store the extracted value. Pass NULL if the value should be skipped.
 * @returns None.
 * @OutParam <strong>out_error_arg</strong> A number (u8t) indicating the state of the operation.
 * @OutParamValue [1] Success. The value was unpacked successfully.
 * @OutParamValue [2] Failure. The combined bits exceed the 16-bit boundary.
 */
extern void unstdbitutils_unpackrtlmulti16(
    u16t packet_buffer_arg,
    u8t pairs_count_arg,
    u8t *const out_error_arg,
    ...
);

//! [32-bit]

/**
 * @brief Packs multiple (bits, value) pairs into a 32-bit value, shifting left-to-right (LTR).
 * @param pairs_count_arg Number of (bits, value) pairs to be packed.
 * @param out_error_arg Should be a pointer to a u8t where the error code will be stored. Pass NULL to ignore errors.
 * @param ... Variable argument list of (bits, value) pairs.
 *             - `bits` specifies the number of bits used by `value`.
 *             - `value` is the value to be packed, limited to `bits` bits.
 * @returns The packed 32-bit value.
 * @OutParam <strong>out_error_arg</strong> A number (u8t) indicating the state of the operation.
 * @OutParamValue [1] Success. The value was packed successfully.
 * @OutParamValue [2] Failure. The combined bits exceed the 32-bit boundary.
 * @OutParamValue [3] Failure. A value does not fit in the specified number of bits.
 */
extern u32t unstdbitutils_packltrmulti32(
    u8t pairs_count_arg,
    u8t *const out_error_arg,
    ...
);

/**
 * @brief Packs multiple (bits, value) pairs into a 32-bit value, shifting right-to-left (RTL).
 * @param pairs_count_arg Number of (bits, value) pairs to be packed.
 * @param out_error_arg Should be a pointer to a u8t where the error code will be stored. Pass NULL to ignore errors.
 * @param ... Variable argument list of (bits, value) pairs.
 *             - `bits` specifies the number of bits used by `value`.
 *             - `value` is the value to be packed, limited to `bits` bits.
 * @returns The packed 32-bit value.
 * @OutParam <strong>out_error_arg</strong> A number (u8t) indicating the state of the operation.
 * @OutParamValue [1] Success. The value was packed successfully.
 * @OutParamValue [2] Failure. The combined bits exceed the 32-bit boundary.
 * @OutParamValue [3] Failure. A value does not fit in the specified number of bits.
 */
extern u32t unstdbitutils_packrtlmulti32(
    u8t pairs_count_arg,
    u8t *const out_error_arg,
    ...
);

/**
 * @brief Unpacks a 32-bit value into multiple values based on the (bits, value) pairs, shifting left-to-right (LTR).
 * @param packet_buffer_arg The 32-bit value to unpack.
 * @param pairs_count_arg Number of (bits, value) pairs to be unpacked.
 * @param out_error_arg Should be a pointer to a u8t where the error code will be stored. Pass NULL to ignore errors.
 * @param ... Variable argument list of (bits, value) pairs.
 *             - `bits` specifies the number of bits used by each value.
 *             - `value` is a pointer to store the extracted value. Pass NULL if the value should be skipped.
 * @returns None.
 * @OutParam <strong>out_error_arg</strong> A number (u8t) indicating the state of the operation.
 * @OutParamValue [1] Success. The value was unpacked successfully.
 * @OutParamValue [2] Failure. The combined bits exceed the 32-bit boundary.
 */
extern void unstdbitutils_unpackltrmulti32(
    u32t packet_buffer_arg,
    u8t pairs_count_arg,
    u8t *const out_error_arg,
    ...
);

/**
 * @brief Unpacks a 32-bit value into multiple values based on the (bits, value) pairs, shifting right-to-left (RTL).
 * @param packet_buffer_arg The 32-bit value to unpack.
 * @param pairs_count_arg Number of (bits, value) pairs to be unpacked.
 * @param out_error_arg Should be a pointer to a u8t where the error code will be stored. Pass NULL to ignore errors.
 * @param ... Variable argument list of (bits, value) pairs.
 *             - `bits` specifies the number of bits used by each value.
 *             - `value` is a pointer to store the extracted value. Pass NULL if the value should be skipped.
 * @returns None.
 * @OutParam <strong>out_error_arg</strong> A number (u8t) indicating the state of the operation.
 * @OutParamValue [1] Success. The value was unpacked successfully.
 * @OutParamValue [2] Failure. The combined bits exceed the 32-bit boundary.
 */
extern void unstdbitutils_unpackrtlmulti32(
    u32t packet_buffer_arg,
    u8t pairs_count_arg,
    u8t *const out_error_arg,
    ...
);

//! [64-bit]

/**
 * @brief Packs multiple (bits, value) pairs into a 64-bit value, shifting left-to-right (LTR).
 * @param pairs_count_arg Number of (bits, value) pairs to be packed.
 * @param out_error_arg Should be a pointer to a u8t where the error code will be stored. Pass NULL to ignore errors.
 * @param ... Variable argument list of (bits, value) pairs.
 *             - `bits` specifies the number of bits used by `value`.
 *             - `value` is the value to be packed, limited to `bits` bits.
 * @returns The packed 64-bit value.
 * @OutParam <strong>out_error_arg</strong> A number (u8t) indicating the state of the operation.
 * @OutParamValue [1] Success. The value was packed successfully.
 * @OutParamValue [2] Failure. The combined bits exceed the 64-bit boundary.
 * @OutParamValue [3] Failure. A value does not fit in the specified number of bits.
 */
extern u64t unstdbitutils_packltrmulti64(
    u8t pairs_count_arg,
    u8t *const out_error_arg,
    ...
);

/**
 * @brief Packs multiple (bits, value) pairs into a 64-bit value, shifting right-to-left (RTL).
 * @param pairs_count_arg Number of (bits, value) pairs to be packed.
 * @param out_error_arg Should be a pointer to a u8t where the error code will be stored. Pass NULL to ignore errors.
 * @param ... Variable argument list of (bits, value) pairs.
 *             - `bits` specifies the number of bits used by `value`.
 *             - `value` is the value to be packed, limited to `bits` bits.
 * @returns The packed 64-bit value.
 * @OutParam <strong>out_error_arg</strong> A number (u8t) indicating the state of the operation.
 * @OutParamValue [1] Success. The value was packed successfully.
 * @OutParamValue [2] Failure. The combined bits exceed the 64-bit boundary.
 * @OutParamValue [3] Failure. A value does not fit in the specified number of bits.
 */
extern u64t unstdbitutils_packrtlmulti64(
    u8t pairs_count_arg,
    u8t *const out_error_arg,
    ...
);

/**
 * @brief Unpacks a 64-bit value into multiple values based on the (bits, value) pairs, shifting left-to-right (LTR).
 * @param packet_buffer_arg The 64-bit value to unpack.
 * @param pairs_count_arg Number of (bits, value) pairs to be unpacked.
 * @param out_error_arg Should be a pointer to a u8t where the error code will be stored. Pass NULL to ignore errors.
 * @param ... Variable argument list of (bits, value) pairs.
 *             - `bits` specifies the number of bits used by each value.
 *             - `value` is a pointer to store the extracted value. Pass NULL if the value should be skipped.
 * @returns None.
 * @OutParam <strong>out_error_arg</strong> A number (u8t) indicating the state of the operation.
 * @OutParamValue [1] Success. The value was unpacked successfully.
 * @OutParamValue [2] Failure. The combined bits exceed the 64-bit boundary.
 */
extern void unstdbitutils_unpackltrmulti64(
    u64t packet_buffer_arg,
    u8t pairs_count_arg,
    u8t *const out_error_arg,
    ...
);

/**
 * @brief Unpacks a 64-bit value into multiple values based on the (bits, value) pairs, shifting right-to-left (RTL).
 * @param packet_buffer_arg The 64-bit value to unpack.
 * @param pairs_count_arg Number of (bits, value) pairs to be unpacked.
 * @param out_error_arg Should be a pointer to a u8t where the error code will be stored. Pass NULL to ignore errors.
 * @param ... Variable argument list of (bits, value) pairs.
 *             - `bits` specifies the number of bits used by each value.
 *             - `value` is a pointer to store the extracted value. Pass NULL if the value should be skipped.
 * @returns None.
 * @OutParam <strong>out_error_arg</strong> A number (u8t) indicating the state of the operation.
 * @OutParamValue [1] Success. The value was unpacked successfully.
 * @OutParamValue [2] Failure. The combined bits exceed the 64-bit boundary.
 */
extern void unstdbitutils_unpackrtlmulti64(
    u64t packet_buffer_arg,
    u8t pairs_count_arg,
    u8t *const out_error_arg,
    ...
);


#endif /* UNSTDLIB_UNSTDBITUTILS_H */
