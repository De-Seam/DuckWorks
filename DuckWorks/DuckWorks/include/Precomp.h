#pragma once
// Core headers
#include "Core/Config.h"
#include "Core/Log/Log.h"
#include "Core/Math/Math.h"
#include "Core/Utilities/Utilities.h"

// External header
#include "External/optick/optick.h"

// STD headers
#include <chrono>

// Profiling Macro
#ifdef _PROFILING_ENABLED
#define PROFILE_SCOPE(inName) OPTICK_EVENT(#inName)
#define PROFILE_SCOPE_STRING(inName) OPTICK_EVENT(inName)
#define PROFILE_FUNCTION() PROFILE_SCOPE_STRING(__FUNCSIG__)
#else
#define PROFILE_SCOPE(inName)
#endif
