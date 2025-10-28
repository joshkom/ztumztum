#ifndef UNSTDLIB_UNSTDINTTYPES_H
#define UNSTDLIB_UNSTDINTTYPES_H

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <limits.h>

typedef char i8;
typedef unsigned char u8;
typedef signed char s8;
typedef int i32;
typedef unsigned long ulong;

typedef unsigned short int u16;
typedef signed short int s16;

typedef unsigned int u32;
typedef signed int s32;

// at least 32 bits
typedef unsigned long int u32l;
typedef signed long int s32l;

#if __WORDSIZE == 64 || defined(__x86_64__)
typedef unsigned long int u64;
typedef signed long int s64;

// at least 64 bits
typedef unsigned long long int u64l;
typedef signed long long int s64l;
#else
typedef u32l u64;
typedef s32l s64;
#endif

// an alias for fixed-size/width types
typedef uint8_t u8t;
typedef int8_t s8t;
typedef uint_least8_t u8lt;
typedef int_least8_t s8lt;

typedef uint16_t u16t;
typedef int16_t s16t;
typedef uint_least16_t u16lt;
typedef int_least16_t s16lt;

typedef uint32_t u32t;
typedef int32_t s32t;
typedef uint_least32_t u32lt;
typedef int_least32_t s32lt;

typedef uint64_t u64t;
typedef int64_t s64t;
typedef uint_least64_t u64lt;
typedef int_least64_t s64lt;

typedef size_t sizet;
typedef ssize_t ssizet;

typedef uintmax_t umaxt;
typedef intmax_t smaxt;

typedef uintptr_t uptrt;
typedef intptr_t sptrt;


// [evaluators]
/**
 * @brief Checks whether a data type is signed or unsigned.
 * @type_arg Should be a data type.
 * @returns Whether a data type is signed or unsigned.
 */
#define unstdinttypes_issignedM(type_arg) (((type_arg)(-1)) < ((type_arg) 0))

/**
 * @attention Should only be used with unsigned data types.
 * @note It does not work with signed data types.
 * @type_arg Should be an unsigned data type.
 * @returns The maximum value an unsigned data type of type `type_arg` can possibly hold.
 */
#define unstdinttypes_unsigned_maximumM(type_arg) ~(type_arg)0

/**
 * @attention Should only be used with signed data types.
 * @note It does not work with unsigned data types.
 * @type_arg Should be a signed data type.
 * @returns The maximum value a signed data type of type `type_arg` can possibly hold.
 */
#define unstdinttypes_signed_maximumM(type_arg) unstdinttypes_issignedM(type_arg) ? (type_arg)(((umaxt)1 << (sizeof(type_arg) * CHAR_BIT - 1)) - 1) : unstdinttypes_unsigned_maximumM(type_arg)

/**
 * @attention Should only be used with signed data types.
 * @note It does not work with unsigned data types.
 * @type_arg Should be a signed data type.
 * @returns The minimum value a signed data type of type `type_arg` can possibly hold.
 */
#define unstdinttypes_signed_minimumM(type_arg) unstdinttypes_issignedM(type_arg) ? (type_arg)-((umaxt)1 << (sizeof(type_arg) * CHAR_BIT - 1)) : 0


#endif /* UNSTDLIB_UNSTDINTTYPES_H */
