#include "Precomp.h"
#include "Game/Entity/Player/Player.h"

// Engine includes
#include <Engine/World/World.h>
#include <Engine/Resources/ResourceManager.h>

// Game includes
#include "Game/App/App.h"
#include "Game/Entity/Player/PlayerAnimation.h"

// External includes
#include <External/box2d/box2d.h>

#include "Engine/Renderer/AnimationManager.h"

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
	String texture_path = "Assets/TinySwords/Factions/Knights/Troops/Warrior/Blue/Warrior_Blue.png";
	texture_render_component.mTexture = gResourceManager.GetResource<TextureResource>(texture_path);
	texture_render_component.mSrcRect = {0, 0, 8, 8};
	texture_render_component.mUseSrcRect = true;

	SetupAnimations();

	{
		SDLEventFunction event_function;
		event_function.mEventType = SDL_MOUSEBUTTONDOWN;
		event_function.mFunctionPtr = [this](const SDL_Event& inEvent)
		{
			OnMouseDown(inEvent);
		};
		mSDLEventFunctions.emplace_back(gSDLEventManager.AddEventFunction(event_function));
	}
	{
		SDLEventFunction event_function;
		event_function.mEventType = SDL_MOUSEBUTTONUP;
		event_function.mFunctionPtr = [this](const SDL_Event& inEvent)
		{
			OnMouseUp(inEvent);
		};
		mSDLEventFunctions.emplace_back(gSDLEventManager.AddEventFunction(event_function));
	}

	AddComponent<HealthComponent>();
}

void Player::Update(float inDeltaTime)
{
	Base::Update(inDeltaTime);


	fm::vec2 moving_direction = {0.f, 0.f};
	const Uint8* key_states = SDL_GetKeyboardState(nullptr);

	moving_direction.y -= Cast<float>(key_states[SDL_SCANCODE_W]);
	moving_direction.y += Cast<float>(key_states[SDL_SCANCODE_S]);
	moving_direction.x += Cast<float>(key_states[SDL_SCANCODE_D]);
	moving_direction.x -= Cast<float>(key_states[SDL_SCANCODE_A]);

	fm::vec2 velocity = GetVelocity();
	fm::vec2 velocity_increment = moving_direction * fm::vec2(mVelocityIncrement * inDeltaTime);
	fm::vec2 new_velocity = velocity + velocity_increment;
	new_velocity = clamp2(new_velocity, -mMaxVelocity, mMaxVelocity);
	SetVelocity(new_velocity);
}

enum class EPlayerAnimationState : uint16
{
	Idle,
	Walking
};

void Player::SetupAnimations()
{
	AnimationComponent& animation_component = AddComponent<AnimationComponent>();
	animation_component.mAnimation = gAnimationManager.CreateAnimation<PlayerAnimation>(this);
	AnimationBase::Frame frame = animation_component.mAnimation->GetFrame(Cast<uint16>(EPlayerAnimationState::Idle), 0);
	frame.mFunctionPtr = [this]()
	{
		b2World* physics_world = mWorld->GetPhysicsWorld();
		b2AABB aabb;
		aabb.lowerBound = GetPhysicsBody()->GetPosition() - b2Vec2(1.f, 1.f);
		aabb.upperBound = GetPhysicsBody()->GetPosition() + b2Vec2(1.f, 1.f);
		physics_world->QueryAABB([this](b2Fixture* inFixture)
			{
			// This is a lambda function that is called for each fixture found in the query.
			// If it returns true then the query will continue, otherwise it will stop.
			// This is useful for finding the first fixture that matches a condition.
			// In this case we are looking for the ground.
			b2Body* body = inFixture->GetBody();
			if (body->GetType() == b2_staticBody)
			{
				// We found the ground, so we can stop the query.
				return false;
			}
			return true;
		}, GetPhysicsBody()->GetAABB());
	};
}

void Player::OnMouseDown(const SDL_Event& inEvent)
{
	if (inEvent.button.button == SDL_BUTTON_LEFT)
	{
		mAttacking = true;
	}
}

void Player::OnMouseUp(const SDL_Event& inEvent)
{
	if (inEvent.button.button == SDL_BUTTON_LEFT)
	{
		mAttacking = false;
	}
}
