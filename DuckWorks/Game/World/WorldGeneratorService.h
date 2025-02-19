#pragma once
#include <DuckCore/Containers/HashMap.h>
#include <DuckCore/Containers/UniquePtr.h>

#include <Game/Entity/Entity.h>
#include <Game/World/EntityWorldGenerationRulesResource.h>
#include <Game/World/Service.h>

class WorldGeneratorService : public Service
{
	RTTI_CLASS(WorldGeneratorService, Service)
public:
	explicit WorldGeneratorService(World& inWorld);

	void Generate();

private:
	struct EntityWorldGenerationRulesResources
	{
		DC::Ref<EntityWorldGenerationRulesResource> mResource;
		std::function<DC::Ref<Entity>(World& inWorld)> mEntityConstructorFunc;
	};
	DC::Array<EntityWorldGenerationRulesResources> mEntityWorldGenerationRulesResources;
};
