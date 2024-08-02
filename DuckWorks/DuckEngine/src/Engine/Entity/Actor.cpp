#include <Engine/Precomp.h>
#include <Engine/Entity/Actor.h>

// Engine includes
#include <Engine/Engine.h>
#include <Engine/Entity/Components/TextureRenderComponent.h>
#include <Engine/Entity/Components/TransformComponent.h>
#include <Engine/Resources/ResourceManager.h>

void Actor::OnAddedToWorld(World* inWorld)
{
	Entity::OnAddedToWorld(inWorld);

	TextureRenderComponent& texture_render_component = AddComponent<TextureRenderComponent>();
	texture_render_component.SetTexture(gEngine->GetManager<ResourceManager>.Get<TextureResource>("Assets/top.jpg"));

	TransformComponent& transform_component = AddComponent<TransformComponent>();
}
