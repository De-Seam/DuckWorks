#include <Engine/Precomp.h>
#include <Engine/World/World.h>

// Engine includes
#include <Engine/Engine.h>
#include <Engine/Entity/Actor.h>
#include <Engine/Entity/Entity.h>
#include <Engine/Entity/Components/TextureRenderComponent.h>
#include <Engine/Entity/Components/TransformComponent.h>

World::World() 
{
	mRegistry.on_construct<TransformComponent>().connect<&World::OnTransformComponentCreated>(this);

	AddEntity(new Actor);
}

void World::Update(float inDeltaTime) 
{
	PROFILE_SCOPE(World::Update)

	mRegistry.clear<TransformUpdatedTag>();

	mIsUpdatingEntities = true;

	for (Entity* entity : mEntities)
		entity->Update(inDeltaTime);

	mIsUpdatingEntities = false;

	for (const Ref<Entity>& entity : mEntitiesToAdd)
		AddEntity(entity);

	for (const Ref<Entity>& entity : mEntitiesToRemove)
		RemoveEntity(entity);
}

void World::Render() 
{
	PROFILE_SCOPE(World::Render)

	auto transform_updated_view = mRegistry.view<TransformComponent, TransformUpdatedTag, TextureRenderComponent>();
	for (entt::entity entity_handle : transform_updated_view)
	{
		TransformComponent& transform_c = transform_updated_view.get<TransformComponent>(entity_handle);
		TextureRenderComponent& texture_render_c = transform_updated_view.get<TextureRenderComponent>(entity_handle);
		texture_render_c.SetTransform(transform_c.GetTransform());
	}

	auto view = mRegistry.view<TextureRenderComponent>();
	for (entt::entity entity_handle : view)
	{
		TextureRenderComponent& texture_render_c = view.get<TextureRenderComponent>(entity_handle);

		gEngine->GetRenderer().Draw(texture_render_c.GetRectangle());
	}
}

void World::AddEntity(const Ref<Entity>& inEntity) 
{
	if (mIsUpdatingEntities)
	{
		mEntitiesToAdd.push_back(inEntity);
		return;
	}
	mEntities.push_back(inEntity);
	inEntity->OnAddedToWorld(this);
}

void World::RemoveEntity(const Ref<Entity>& inEntity) 
{
	if (mIsUpdatingEntities)
	{
		mEntitiesToRemove.push_back(inEntity);
		return;
	}
	Array<Ref<Entity>>::iterator it = std::ranges::find(mEntities.begin(), mEntities.end(), inEntity);
	gSwap(*it, mEntities.back());
	Ref<Entity> entity = mEntities.back();
	mEntities.pop_back();
	entity->OnRemovedFromWorld(this);
}

void World::OnTransformComponentCreated(entt::registry& inRegistry, entt::entity inEntityHandle) 
{
	inRegistry.emplace_or_replace<TransformUpdatedTag>(inEntityHandle);
}
