#pragma once
#include "Core/CoreBase.h"

#define STRING_BUFFER_SIZE 256

struct PhysicsComponent;
// Assumes a imgui window is created.
// Return true if the value was modified
bool gDebugDrawJson(Json& ioJson, const String& inLabel);

// Return true if the value was modified
bool gHandleKeyValuePair(Json& ioJson, const String& inLabel, const String& inKey, Json& ioValue, bool inSameLine = false);

void gDrawEntityPhysicsOutline(const PhysicsComponent& inPhysicsComponent, const fm::vec4& inColor = fm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
