#pragma once
#include <Core/Config.h>
#include <External/optick/optick.h>

// Profiling Macro
#ifdef _PROFILING_ENABLED
#define PROFILE_SCOPE(inName) OPTICK_EVENT(#inName)
#define PROFILE_CATEGORY(inName) OPTICK_CATEGORY(#inName)
#define PROFILE_FRAME(inName) OPTICK_FRAME(#inName)
#define PROFILE_THREAD(inName) OPTICK_THREAD(#inName)
#else
#define PROFILE_SCOPE(inName)
#define PROFILE_CATEGORY(inName)
#define PROFILE_FRAME(inName)
#define PROFILE_THREAD(inName)
#endif