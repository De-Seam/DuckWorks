#include <Precomp.h>
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

	AddNode(new Actor);
}

World::~World() 
{
	for (Ref<Node>& node : mNodes)
		node->OnRemovedFromWorld(this);
	mNodes.clear();
}

void World::Update(float inDeltaTime) 
{
	PROFILE_SCOPE(World::Update)

	mRegistry.clear<TransformUpdatedTag>();

	mIsUpdatingEntities = true;

	for (Node* node : mNodes)
	{
		if (Entity* entity = node->As<Entity>())
			entity->Update(inDeltaTime);
	}

	mIsUpdatingEntities = false;
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

void World::AddNode(const Ref<Node>& inNode) 
{
	mNodes.push_back(inNode);
	inNode->OnAddedToWorld(this);
}

void World::RemoveNode(const Node& inNode) 
{
	for (Array<Ref<Node>>::iterator iterator = mNodes.begin(); iterator != mNodes.end(); iterator++)
	{
		if (*iterator == &inNode)
		{
			(*iterator)->OnRemovedFromWorld(this);
			mNodes.erase(iterator);
			break;
		}
	}
}

void World::OnTransformComponentCreated(entt::registry& inRegistry, entt::entity inEntityHandle) 
{
	inRegistry.emplace_or_replace<TransformUpdatedTag>(inEntityHandle);
}
