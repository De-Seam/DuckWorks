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

#include "Engine/Events/EventManager.h"
#include "Engine/Renderer/AnimationManager.h"

Json Player::Serialize() const
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mVelocityIncrement);
	JSON_SAVE(json, mMaxVelocity);

	return json;
}

void Player::Deserialize(const Json& inJson)
{
	PROFILE_SCOPE(World::Deserialize)

	JSON_TRY_LOAD(inJson, mVelocityIncrement);
	JSON_TRY_LOAD(inJson, mMaxVelocity);

	Base::Deserialize(inJson);
}

Player::Player(World* inWorld)
	: Actor(inWorld)
{
	AddComponent<HealthComponent>();
	CameraComponent& camera_component = AddComponent<CameraComponent>();
	camera_component.mIsActive = true;

	SetHalfSize(fm::vec2(100.0f, 100.0f));

	TextureRenderComponent& texture_render_component = AddComponent<TextureRenderComponent>();
	String texture_path = "Assets/TinySwords/Factions/Knights/Troops/Warrior/Blue/Warrior_Blue.png";
	texture_render_component.mTexture = gResourceManager.GetResource<TextureResource>(texture_path);
	texture_render_component.mSrcRect = {0, 0, 192, 192};
	texture_render_component.mUseSrcRect = true;

	SetupAnimations();

	{
		EventManager::EventFunction event_function;
		event_function.mEventType = EventType::MouseButtonDown;
		event_function.mFunctionPtr = [this](const EventManager::EventData& inData)
		{
			OnMouseDown(inData);
		};
		mEventFunctions.emplace_back(gEventManager.AddEventFunction(event_function));
	}
	{
		EventManager::EventFunction event_function;
		event_function.mEventType = EventType::MouseButtonUp;
		event_function.mFunctionPtr = [this](const EventManager::EventData& inData)
		{
			OnMouseUp(inData);
		};
		mEventFunctions.emplace_back(gEventManager.AddEventFunction(event_function));
	}

	AddComponent<PhysicsPositionOrRotationUpdatedTag>();
	AddComponent<PhysicsSizeUpdatedTag>();

	CollisionComponent& collision_component = AddComponent<CollisionComponent>();
	CollisionObject::InitParams params;
	params.mTransform = GetTransform();
	params.mType = CollisionObject::Type::Static;
	collision_component.mCollisionObjectHandle = GetWorld()->GetCollisionWorld()->CreateCollisionObject(params);
}

void Player::BeginPlay()
{
	Base::BeginPlay();
}

void Player::Update(float inDeltaTime)
{
	PROFILE_SCOPE(Player::Update)

	Base::Update(inDeltaTime);


	fm::vec2 moving_direction = {0.f, 0.f};
	const Uint8* key_states = SDL_GetKeyboardState(nullptr);

	moving_direction.y -= Cast<float>(key_states[SDL_SCANCODE_W]);
	moving_direction.y += Cast<float>(key_states[SDL_SCANCODE_S]);
	moving_direction.x += Cast<float>(key_states[SDL_SCANCODE_D]);
	moving_direction.x -= Cast<float>(key_states[SDL_SCANCODE_A]);

	fm::Transform2D& transform = GetComponent<TransformComponent>().mTransform;
	fm::vec2 position = transform.position;
	position += moving_direction * mVelocityIncrement * inDeltaTime;
	GetWorld()->GetCollisionWorld()->MoveTo(GetComponent<CollisionComponent>().mCollisionObjectHandle, position);
	transform.position = GetWorld()->GetCollisionWorld()->GetCollisionObject(GetComponent<CollisionComponent>().mCollisionObjectHandle).GetTransform().position;

	//fm::vec2 velocity = GetVelocity();
	//fm::vec2 velocity_increment = moving_direction * fm::vec2(mVelocityIncrement * inDeltaTime);
	//fm::vec2 new_velocity = velocity + velocity_increment;
	//new_velocity = clamp2(new_velocity, -mMaxVelocity, mMaxVelocity);
	//SetVelocity(new_velocity);
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
	//AnimationBase::Frame frame = animation_component.mAnimation->GetFrame(Cast<uint16>(EPlayerAnimationState::Idle), 0);
	//frame.mFunctionPtr = [this]()
	//{
	//	b2World* physics_world = mWorld->GetPhysicsWorld();
	//	b2AABB aabb;
	//	aabb.lowerBound = GetPhysicsBody()->GetPosition() - b2Vec2(1.f, 1.f);
	//	aabb.upperBound = GetPhysicsBody()->GetPosition() + b2Vec2(1.f, 1.f);
	//	physics_world->QueryAABB([this](b2Fixture* inFixture)
	//		{
	//		// This is a lambda function that is called for each fixture found in the query.
	//		// If it returns true then the query will continue, otherwise it will stop.
	//		// This is useful for finding the first fixture that matches a condition.
	//		// In this case we are looking for the ground.
	//		b2Body* body = inFixture->GetBody();
	//		if (body->GetType() == b2_staticBody)
	//		{
	//			// We found the ground, so we can stop the query.
	//			return false;
	//		}
	//		return true;
	//	}, GetPhysicsBody()->GetAABB());
	//};
}

void Player::OnMouseDown(const EventManager::EventData& inData)
{
	if (inData.mMouseDown.mMouseButton == MouseButton::Left)
	{
		mAttacking = true;
	}
}

void Player::OnMouseUp(const EventManager::EventData& inData)
{
	if (inData.mMouseUp.mMouseButton == MouseButton::Left)
	{
		mAttacking = false;
	}
}
