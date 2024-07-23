#pragma once

// Platform-specific includes and definitions
#if defined(_MSC_VER) // Microsoft Visual C++
#define DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) // GCC and Clang
#define DEBUG_BREAK() __builtin_trap()
#else
#define DEBUG_BREAK() *(volatile int*)0 = 0 // Trigger an access violation
#endif

#ifdef _DEBUG
// Define the macros for 1 and 2 arguments
#define gAssert(inCondition) \
    do { \
        if (!(inCondition)) { \
            DEBUG_BREAK(); \
        } \
    } while (0)
#else
#define gAssert(inCondition)
#endif
