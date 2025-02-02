#include "Precomp.h"
#include "Engine/Factory/Factory.h"

#include "Engine/Debug/Windows/DebugUIWindow.h"

Factory<EntityComponent> gEntityComponentFactory = {};
Factory<Entity> gEntityFactory = {};
Factory<DebugUIWindow> gDebugUIWindowFactory = {};
