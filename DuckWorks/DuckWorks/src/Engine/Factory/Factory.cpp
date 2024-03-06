#include "Precomp.h"
#include "Engine/Factory/Factory.h"

Factory<Entity, World*> gEntityFactory = {};
Factory<DebugUIWindow> gDebugUIWindowFactory = {};
ComponentFactory gComponentFactory = {};
