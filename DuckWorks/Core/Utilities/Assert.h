#pragma once

// Core includes
#include <Core/Config.h>

// Std includes
#include <cassert>

// Platform-specific includes and definitions
#if defined(_MSC_VER) // Microsoft Visual C++
#define DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) // GCC and Clang
#define DEBUG_BREAK() __builtin_trap()
#else
#define DEBUG_BREAK() *(volatile int*)0 = 0 // Trigger an access violation
#endif

#ifdef _ASSERTS_ENABLED
// Define the macros for 1 and 2 arguments
#define gAssert(inCondition) assert(inCondition)
#else
#define gAssert(inCondition)
#endif
