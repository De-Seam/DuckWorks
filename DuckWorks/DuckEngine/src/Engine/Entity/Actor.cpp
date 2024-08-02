#include <Engine/Precomp.h>
#include <Engine/Entity/Actor.h>

// Engine includes
#include <Engine/Engine.h>
#include <Engine/Entity/Components/TextureRenderComponent.h>
#include <Engine/Entity/Components/TransformComponent.h>
#include <Engine/Resources/ResourceManager.h>

#include <External/SFML/Window/Keyboard.hpp>

void Actor::Update(float inDeltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		const Transform2D& transform = GetComponent<TransformComponent>().GetTransform();
		GetComponent<TransformComponent>().SetTransform(Transform2D(transform.mPosition + Vec2(-1.0f, 0.0f) * inDeltaTime, transform.mScale, transform.mRotation), GetRegistry(), GetEntityHandle());
	}
}

void Actor::OnAddedToWorld(World* inWorld)
{
	Entity::OnAddedToWorld(inWorld);

	TextureRenderComponent& texture_render_component = AddComponent<TextureRenderComponent>();
	texture_render_component.SetTexture(gEngine->GetManager<ResourceManager>().Get<TextureResource>("Assets/top.jpg"));
	texture_render_component.SetSize(Vec2(100.0f, 100.0f));

	AddComponent<TransformComponent>();
}
