#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define P_WINDOWS 1
#ifndef _WIN64
#error "64-bit is required on Windows!"
#endif
#elif defined(__linux__) || defined(__gnu_linux__)
// Linux OS
#define P_LINUX 1
#if defined(__ANDROID__)
#define P_ANDROID 1
#endif
#elif defined(__unix__)
// Catch anything not caught by the above.
#define P_UNIX 1
#elif defined(_POSIX_VERSION)
// Posix
#define P_POSIX 1
#elif __APPLE__
// Apple platforms
#define P_APPLE 1
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#define P_IOS 1
#define P_IOS_SIMULATOR 1
#elif TARGET_OS_IPHONE
#define P_IOS 1
// iOS device
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#else
#error "Unknown Apple platform"
#endif
#else
#error "Unknown platform!"
#endif

#ifdef _DEBUG
#define P_DEBUG
#endif // _DEBUG
