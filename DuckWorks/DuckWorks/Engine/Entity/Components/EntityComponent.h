#pragma once
// Core includes
#include <Core/RTTI/RTTI.h>

class EntityComponent : public RTTIClass
{
	RTTI_CLASS(EntityComponent, RTTIClass)
};

class EntityComponentTag : public EntityComponent
{
	RTTI_CLASS(EntityComponentTag, EntityComponent)
};