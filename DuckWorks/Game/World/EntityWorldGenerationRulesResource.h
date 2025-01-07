#pragma once
#include <Engine/Resources/Resource.h>

class EntityWorldGenerationRulesResource : public Resource
{
	RTTI_CLASS(EntityWorldGenerationRulesResource, Resource)
public:
	EntityWorldGenerationRulesResource(const DC::Json& inJson);

private:
	float mWeight = 1.0f; // Weight, in range 0-1. 0 means it will never be selected, 1 means it is quite common.
	DC::UVec2 mGroupSizeMinMax = { 1, 20 }; // Min and max size of a group of entities.
};
