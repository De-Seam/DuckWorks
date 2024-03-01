#pragma once
// Core headers
#include "Core/Math/FMath.h"
#include "Core/Log/Log.h"
#include "Core/Utilities/Utilities.h"

// External header
#include "External/optick/optick.h"

// STD headers
#include <chrono>

#ifdef _SHIP
#define IMGUI(x)
#else
#define IMGUI(x) x
#endif
