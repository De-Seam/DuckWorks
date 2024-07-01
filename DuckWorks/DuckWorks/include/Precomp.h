#pragma once
// Core headers
#include "Core/Log/Log.h"
#include "Core/Math/FMath.h"
#include "Core/Utilities/Utilities.h"

// External header
#include "External/optick/optick.h"

// STD headers
#include <chrono>

// Disable all development ifdefs in SHIP mode
#ifdef _SHIP
#else
#define _IMGUI_ENABLED 
#define _PROFILING_ENABLED 
#endif

// ImGui Macro
#ifdef _IMGUI_ENABLED
#define IMGUI(x) x
#else
#define IMGUI(x)
#endif

// Profiling Macro
#ifdef _PROFILING_ENABLED
#define PROFILE_SCOPE(inName) OPTICK_EVENT(#inName)
#define PROFILE_SCOPE_STRING(inName) OPTICK_EVENT(inName)
#define PROFILE_FUNCTION() PROFILE_SCOPE_STRING(__FUNCSIG__)
#else
#define PROFILE_SCOPE(inName)
#endif
