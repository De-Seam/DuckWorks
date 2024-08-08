#pragma once
#pragma once
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

// Assert macro
#ifdef _DEBUG
#define _ASSERTS_ENABLED
#endif

#define ENABLE_SIMD