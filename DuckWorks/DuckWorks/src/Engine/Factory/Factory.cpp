#include "Precomp.h"
#include "Engine/Factory/Factory.h"

#include "Engine/Debug/Windows/DebugUIWindow.h"

Factory<Entity> gEntityFactory = {};
EntityComponentFactory gEntityComponentFactory = {};
Factory<DebugUIWindow> gDebugUIWindowFactory = {};
