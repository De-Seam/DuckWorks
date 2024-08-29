#include <Precomp.h>
#include <Engine/Entity/Actor.h>

// Engine includes
#include <Engine/Engine.h>
#include <Engine/Entity/Components/TextureRenderComponent.h>
#include <Engine/Entity/Components/TransformComponent.h>
#include <Engine/Resources/ResourceManager.h>

#include <External/SFML/Window/Keyboard.hpp>

void Actor::Update(float inDeltaTime)
{
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	//{
	//	const Transform2D& transform = GetLocalTransform();
	//	SetLocalTransform(Transform2D(transform.mPosition + Vec2(-1.0f, 0.0f) * inDeltaTime, transform.mScale, transform.mRotation));
	//}
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	//{
	//	const Transform2D& transform = GetLocalTransform();
	//	SetLocalTransform(Transform2D(transform.mPosition - Vec2(-1.0f, 0.0f) * inDeltaTime, transform.mScale, transform.mRotation));
	//}
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	//{
	//	const Transform2D& transform = GetLocalTransform();
	//	SetLocalTransform(Transform2D(transform.mPosition + Vec2(0.0f, -1.0f) * inDeltaTime, transform.mScale, transform.mRotation));
	//}
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	//{
	//	const Transform2D& transform = GetLocalTransform();
	//	SetLocalTransform(Transform2D(transform.mPosition - Vec2(0.0f, -1.0f) * inDeltaTime, transform.mScale, transform.mRotation));
	//}
}

void Actor::OnAddedToWorld(World* inWorld)
{
	Base::OnAddedToWorld(inWorld);

	TextureRenderComponent& texture_render_component = AddComponent<TextureRenderComponent>();
	texture_render_component.SetTexture(gEngine->GetManager<ResourceManager>().Get<TextureResource>("Assets/top.jpg"));
	texture_render_component.SetSize(Vec2(128.0f, 128.0f));

	AddComponent<TransformComponent>();
}
