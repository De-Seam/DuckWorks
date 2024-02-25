#include "Precomp.h"
#include "Game/Entity/Player/Player.h"

// Engine includes
#include <Engine/World/World.h>
#include <Engine/Resources/ResourceManager.h>

// Game includes
#include "Game/App/App.h"

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
	GetPhysicsBody()->SetFixedRotation(true);
	GetPhysicsBody()->SetLinearDamping(5.f);

	TextureRenderComponent& texture_render_component = AddComponent<TextureRenderComponent>();
	texture_render_component.mTexture = gResourceManager.GetResource<TextureResource>("Assets/top.jpg");

	SDLEventFunction event_function;
	event_function.mEventType = SDL_KEYDOWN;
	event_function.mFunctionPtr = [this](const SDL_Event& inEvent)
	{
		OnKeyDown(inEvent);
	};
	mSDLEventFunction = gSDLEventManager.AddEventFunction(event_function);
}

void Player::Update(float inDeltaTime)
{
	Base::Update(inDeltaTime);


	fm::vec2 moving_direction = {0.f, 0.f};
	const Uint8* key_states = SDL_GetKeyboardState(nullptr);
	if (key_states[SDL_SCANCODE_W])
	{
		moving_direction.y -= 1.f;
	}
	if (key_states[SDL_SCANCODE_S])
	{
		moving_direction.y += 1.f;
	}
	if (key_states[SDL_SCANCODE_A])
	{
		moving_direction.x -= 1.f;
	}
	if (key_states[SDL_SCANCODE_D])
	{
		moving_direction.x += 1.f;
	}
	fm::vec2 velocity = GetVelocity();
	fm::vec2 velocity_increment = moving_direction * fm::vec2(mVelocityIncrement * inDeltaTime);
	fm::vec2 new_velocity = velocity + velocity_increment;
	new_velocity = clamp2(new_velocity, -mMaxVelocity, mMaxVelocity);
	SetVelocity(new_velocity);
}

void Player::OnKeyDown(const SDL_Event& inEvent)
{}
