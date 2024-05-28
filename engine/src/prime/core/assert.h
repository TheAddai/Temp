#pragma once

#include "logger.h"
#include "defines.h"

#ifdef P_DEBUG

#define P_ASSERT(expr)                                              \
    {                                                                \
        if (expr) {                                                  \
        } else {                                                     \
            P_ERROR("Assertion {0} failed in {1} at line {2}", P_STR(expr), P_FILE, P_LINE);                \
            P_BREAK;                                                \
        }                                                            \
    }

#define P_ASSERT_MSG(expr, message)                                 \
    {                                                                \
        if (expr) {                                                  \
        } else {                                                     \
            P_ERROR("Assertion {0} failed in {1} at line {2} \n{3}", P_STR(expr), P_FILE, P_LINE, message);                \
            P_BREAK;                                                \
        }                                                            \
    }
#else
#define P_ASSERT(expr)               void()0
#define P_ASSERT_MSG(expr, message)  void()0
#endif // P_DEBUG
