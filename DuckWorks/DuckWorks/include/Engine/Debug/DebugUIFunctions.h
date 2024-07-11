#pragma once
#include "Core/CoreBase.h"

#ifdef _DEBUG

struct AABB;

#define STRING_BUFFER_SIZE 256

// Assumes a imgui window is created.
// Return true if the value was modified
bool gDebugDrawJson(Json& ioJson, const String& inLabel);

// Return true if the value was modified
bool gHandleKeyValuePair(Json& ioJson, const String& inLabel, const String& inKey, Json& ioValue, bool inSameLine = false, bool inShowKey = true);

#endif
