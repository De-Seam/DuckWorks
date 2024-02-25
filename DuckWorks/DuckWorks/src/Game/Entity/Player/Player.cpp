#include "Precomp.h"
#include "Game/Entity/Player/Player.h"

// Engine includes
#include <Engine/World/World.h>
#include <Engine/Resources/ResourceManager.h>

// External includes
#include <External/box2d/box2d.h>

Player::Player(World* inWorld)
	: Base(inWorld)
{
	SetHalfSize(fm::vec2(100.0f, 100.0f));

	//b2BodyDef body_def;
	//body_def.type = b2_dynamicBody;
	//body_def.position.Set(GetPosition().x, GetPosition().y);
	//body_def.angle = 0.0f;
	//
	//b2PolygonShape dynamic_box;
	//dynamic_box.SetAsBox(GetHalfSize().x, GetHalfSize().y);
	//
	//b2FixtureDef fixture_def;
	//fixture_def.shape = &dynamic_box;
	//fixture_def.density = 100;

	//CreatePhysicsBody(body_def, fixture_def);
	CreateDefaultPhysicsBody();

	TextureRenderComponent& texture_render_component = AddComponent<TextureRenderComponent>();
	texture_render_component.mTexture = gResourceManager.GetResource<TextureResource>("Assets/top.jpg");

	SDLEventFunction event_function;
	event_function.mEventType = SDL_KEYDOWN;
	event_function.mFunctionPtr = [=](const SDL_Event& inEvent)
	{
		this->OnKeyDown(inEvent);
	};
	mSDLEventFunction = gSDLEventManager.AddEventFunction(event_function);
}

void Player::Update(float inDeltaTime)
{}

void Player::OnKeyDown(const SDL_Event& inEvent)
{
	if (inEvent.key.keysym.sym == SDLK_SPACE)
	{
		SetVelocity({0.0f, -100.0f});
	}
}
