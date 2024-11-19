#include <Game/Entity/Systems/RenderSystem.h>

#include <Engine/Engine.h>
#include <Engine/Renderer/Renderer.h>

#include <Game/Entity/Components/GameplayComponents.h>
#include <Game/Entity/Components/RenderComponent.h>
#include <Game/World/World.h>

void RenderSystem::Update(float inDeltaTime)
{
	Renderer& renderer = gEngine->GetManager<Renderer>();

	// Update all renderable entities
	auto& registry = GetRegistry();
	registry.view<const SpriteRenderComponent, const TransformComponent>().each([&renderer](entt::entity inEntity, const SpriteRenderComponent& inSpriteRenderComponent, const TransformComponent& inTransformComponent)
	{
		gAssert(inSpriteRenderComponent.mSprite != nullptr);
		renderer.DrawSprite(*inSpriteRenderComponent.mSprite, inTransformComponent.mTransform);
	});
}
