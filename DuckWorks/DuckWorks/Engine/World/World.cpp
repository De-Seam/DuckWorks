#include <Precomp.h>
#include <Engine/World/World.h>

// Engine includes
#include <Engine/Engine.h>
#include <Engine/Collision/Grid.h>
#include <Engine/Entity/Actor.h>
#include <Engine/Entity/Entity.h>
#include <Engine/Entity/Components/TextureRenderComponent.h>
#include <Engine/Entity/Components/TransformComponent.h>

Actor* gActor = nullptr;

World::World() 
{
	mRegistry.on_construct<TransformComponent>().connect<&World::OnTransformComponentCreated>(this);

	mGrid = std::make_unique<Grid>(IVec2(-10000, -10000), IVec2(1000, 1000), IVec2(64, 64));

	Actor* actor = new Actor;
	AddEntity(actor);
}

World::~World() 
{
	mEntities.clear();
}

void World::Update(float inDeltaTime) 
{
	PROFILE_SCOPE(World::Update)

	mIsUpdatingEntities = true;

	for (Entity* entity : mEntities)
		entity->Update(inDeltaTime);

	mIsUpdatingEntities = false;
}

void World::Render() 
{
	PROFILE_SCOPE(World::Render)

	{
		PROFILE_SCOPE(World::Render::SyncTransforms)
		auto transform_updated_view = mRegistry.view<TransformComponent, TransformUpdatedTag, TextureRenderComponent>();
		for (entt::entity entity_handle : transform_updated_view)
		{
			TransformComponent& transform_c = transform_updated_view.get<TransformComponent>(entity_handle);
			TextureRenderComponent& texture_render_c = transform_updated_view.get<TextureRenderComponent>(entity_handle);
			texture_render_c.SetTransform(transform_c.GetTransform());
		}
	}

	mRegistry.clear<TransformUpdatedTag>();

	{
		PROFILE_SCOPE(World::Render::Drawing)
		auto view = mRegistry.view<TextureRenderComponent>();
		for (entt::entity entity_handle : view)
		{
			TextureRenderComponent& texture_render_c = view.get<TextureRenderComponent>(entity_handle);

			gEngine->GetRenderer().Draw(texture_render_c.GetRectangle());
		}
	}

	mGrid->Render();
}

void World::AddEntity(const Ref<Entity>& inEntity)
{
	mEntities.push_back(inEntity);
	inEntity->OnAddedToWorld(this);
}

void World::RemoveEntity(Entity& inEntity)
{
	inEntity.OnRemovedFromWorld();
	for (auto it = mEntities.begin(); it != mEntities.end(); ++it)
	{
		if ((*it).Get() == &inEntity)
		{
			mEntities.erase(it);
			return;
		}
	}
	gAssert(false && "Entity was not found in th World!");
}

void World::OnTransformComponentCreated(entt::registry& inRegistry, entt::entity inEntityHandle) 
{
	inRegistry.emplace_or_replace<TransformUpdatedTag>(inEntityHandle);
}
