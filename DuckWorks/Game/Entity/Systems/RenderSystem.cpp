#include <Game/Entity/Systems/RenderSystem.h>

#include <Engine/Engine.h>
#include <Engine/Renderer/Renderer.h>

#include <Game/Entity/Components/GameplayComponents.h>
#include <Game/Entity/Components/RenderComponent.h>
#include <Game/World/World.h>

void RenderSystem::Update(float)
{
	entt::registry& registry = GetRegistry();
	Renderer& renderer = gEngine->GetManager<Renderer>();

	// Render all sprites
	registry.view<const SpriteRenderComponent, const TransformComponent>().each([&renderer](const SpriteRenderComponent& inSpriteRenderComponent, const TransformComponent& inTransformComponent)
	{
		gAssert(inSpriteRenderComponent.mSprite != nullptr);
		renderer.DrawSprite(*inSpriteRenderComponent.mSprite, inTransformComponent.mTransform);
	});
}
