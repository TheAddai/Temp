#pragma once

#include "prime/platform/platform_detection.h"

/** @brief Unsigned 8-bit integer */
typedef unsigned char ui8;

/** @brief Unsigned 16-bit integer */
typedef unsigned short ui16;

/** @brief Unsigned 32-bit integer */
typedef unsigned int ui32;

/** @brief Unsigned 64-bit integer */
typedef unsigned long long ui64;

// Signed int types.

/** @brief Signed 8-bit integer */
typedef signed char i8;

/** @brief Signed 16-bit integer */
typedef signed short i16;

/** @brief Signed 32-bit integer */
typedef signed int i32;

/** @brief Signed 64-bit integer */
typedef signed long long i64;

// Floating point types

/** @brief 32-bit floating point number */
typedef float f32;

/** @brief 64-bit floating point number */
typedef double f64;

// Boolean types

/** @brief 32-bit boolean type, used for APIs which require it */
typedef int b32;

/** @brief 8-bit boolean type */
typedef bool b8;

/** @brief convert to string */
#define P_STR(x) #x

/** @brief get the line number */
#define P_LINE __LINE__

/** @brief get function name */
#define P_FUNC __FUNCTION__

/** @brief get file name */
#define P_FILE __FILE__

/** @brief make inline */
#define P_INLINE inline 

/** @brief debug breakpoint */
#ifdef P_WINDOWS
#define P_BREAK  __debugbreak();
#else
#define P_BREAK  void()0
#endif // P_WINDOWS