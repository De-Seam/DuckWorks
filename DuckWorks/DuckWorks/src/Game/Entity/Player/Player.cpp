#include "Precomp.h"
#include "Game/Entity/Player/Player.h"

// Core includes
#include "Core/Utilities/RefObject.h"

// Engine includes
#include "Engine/Entity/Components.h"
#include "Engine/Events/EventManager.h"
#include "Engine/Renderer/AnimationManager.h"
#include "Engine/Resources/ResourceManager.h"
#include "Engine/World/World.h"

// Game includes
#include "Game/App/App.h"
#include "Game/Entity/Player/PlayerAnimation.h"

RTTI_CLASS_DEFINITION(Player)

Json Player::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mVelocityIncrement);
	JSON_SAVE(json, mMaxVelocity);

	return json;
}

void Player::Deserialize(const Json& inJson)
{
	JSON_TRY_LOAD(inJson, mVelocityIncrement);
	JSON_TRY_LOAD(inJson, mMaxVelocity);

	Base::Deserialize(inJson);
}

Player::Player(const ConstructParameters& inConstructParameters)
	: Base(inConstructParameters)
{
	AddComponent<HealthComponent>(HealthComponent::ConstructParameters());
	AddComponent<CameraComponent>(CameraComponent::ConstructParameters());
	LoopOverComponents<CameraComponent>([this](CameraComponent& inCameraComponent) { inCameraComponent.mIsActive = false; });

	SetHalfSize(fm::vec2(64.f, 64.f));

	TextureRenderComponent::ConstructParameters texture_render_component_parameters;
	String texture_path = "Assets/TinySwords/Factions/Knights/Troops/Warrior/Blue/Warrior_Blue.png";
	texture_render_component_parameters.mTexture = gResourceManager.GetResource<TextureResource>(texture_path);
	texture_render_component_parameters.mSrcRect = {0, 0, 192, 192};
	texture_render_component_parameters.mUseSrcRect = true;
	AddComponent<TextureRenderComponent>(texture_render_component_parameters);

	SetupAnimations();

	{
		EventManager::EventFunction event_function;
		event_function.mEventType = EventType::MouseButtonDown;
		event_function.mFunctionPtr = [this](const EventManager::EventData& inData) { OnMouseDown(inData); };
		mEventFunctions.emplace_back(gEventManager.AddEventFunction(event_function));
	}
	{
		EventManager::EventFunction event_function;
		event_function.mEventType = EventType::MouseButtonUp;
		event_function.mFunctionPtr = [this](const EventManager::EventData& inData) { OnMouseUp(inData); };
		mEventFunctions.emplace_back(gEventManager.AddEventFunction(event_function));
	}

	LoopOverComponents<CollisionComponent>(
		[this](CollisionComponent& inCollisionComponent)
		{
			CollisionObjectWrapper collision_object = GetWorld()->GetCollisionWorld()->GetCollisionObject(inCollisionComponent.mCollisionObjectHandle);

			collision_object->SetType(CollisionObject::EType::Dynamic);
			collision_object->SetOnCollisionFunc([this](const CollisionFuncParams& inParams) { OnCollision(inParams); });
		});
}

void Player::Init(const InitParams& inInitParams)
{
	Base::Init(inInitParams);
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

	moving_direction.y -= SCast<float>(key_states[SDL_SCANCODE_W]);
	moving_direction.y += SCast<float>(key_states[SDL_SCANCODE_S]);
	moving_direction.x += SCast<float>(key_states[SDL_SCANCODE_D]);
	moving_direction.x -= SCast<float>(key_states[SDL_SCANCODE_A]);

	if (mVelocity.x > 0.f)
	{
		mVelocity.x -= mVelocityDecrement.x * inDeltaTime;
		mVelocity.x = std::max(mVelocity.x, 0.f);
	}
	else if (mVelocity.x < 0.f)
	{
		mVelocity.x += mVelocityDecrement.x * inDeltaTime;
		mVelocity.x = std::min(mVelocity.x, 0.f);
	}

	if (mVelocity.y > 0.f)
	{
		mVelocity.y -= mVelocityDecrement.y * inDeltaTime;
		mVelocity.y = std::max(mVelocity.y, 0.f);
	}
	else if (mVelocity.y < 0.f)
	{
		mVelocity.y += mVelocityDecrement.y * inDeltaTime;
		mVelocity.y = std::min(mVelocity.y, 0.f);
	}

	mVelocity += moving_direction * mVelocityIncrement * inDeltaTime;

	mVelocity = clamp2(mVelocity, -mMaxVelocity, mMaxVelocity);

	fm::vec2 position = GetPosition();
	position += mVelocity * inDeltaTime;
	MoveTo(position);
	//transform = GetWorld()->GetCollisionWorld()->MoveTo(GetComponent<CollisionComponent>().mCollisionObjectHandle, position);
	//transform.position = GetWorld()->GetCollisionWorld()->GetCollisionObject(GetComponent<CollisionComponent>().mCollisionObjectHandle).GetTransform().position;

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
	AnimationComponent::ConstructParameters animation_component_parameters;
	animation_component_parameters.mAnimation = gAnimationManager.CreateAnimation<PlayerAnimation>(this);
	AddComponent<AnimationComponent>(animation_component_parameters);
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

void Player::OnCollision(const CollisionFuncParams& inParams)
{
	gLog("Player collided with %u", inParams.mOther.mIndex);
}
