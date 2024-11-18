#include <Game/Entity/Systems/RenderSystem.h>

// Engine includes
#include <Engine/Engine.h>
#include <Engine/Renderer/Renderer.h>

// Game includes
#include <Game/Entity/Components/GameplayComponents.h>
#include <Game/Entity/Components/RenderComponent.h>
#include <Game/World/World.h>

RenderSystem::RenderSystem(World& inWorld) :
	EntitySystem(inWorld)
{
	mUpdateHandle = GetWorld()->RegisterUpdateCallback([this](float inDeltaTime) { Update(inDeltaTime); });
}

void RenderSystem::Update(float inDeltaTime)
{
	Renderer& renderer = gEngine->GetManager<Renderer>();

	// Update all renderable entities
	auto& registry = GetWorld()->GetRegistry();
	registry.view<const SpriteRenderComponent, const TransformComponent>().each([renderer](entt::entity inEntity, const SpriteRenderComponent& inSpriteRenderComponent, const TransformComponent& inTransformComponent)
	{
		gAssert(inSpriteRenderComponent.mSprite != nullptr);
		renderer.DrawSprite(*inSpriteRenderComponent.mSprite, inTransformComponent.mTransform);
	});
}
