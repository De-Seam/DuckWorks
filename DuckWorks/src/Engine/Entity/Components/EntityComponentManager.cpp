#include "Precomp.h"
#include "Engine/Entity/Components/EntityComponentManager.h"

// Engine includes
#include "Engine/Entity/Components/EntityComponent.h"

using namespace DC;

EntityComponentManager gEntityComponentManager;

RTTI_CLASS_DEFINITION(EntityComponentManager, StandardAllocator)

RTTI_EMPTY_SERIALIZE_DEFINITION(EntityComponentManager)
