#include "Precomp.h"
#include "Game/Entity/MovingPlatform.h"

// Engine includes
#include "Engine/Collision/CollisionWorld.h"
#include "Engine/Entity/Components.h"
#include "Engine/Resources/ResourceManager.h"
#include "Engine/World/World.h"

// Game includes
#include "Engine/Entity/Components/CollisionComponent.h"

#include "Game/Entity/Player/Player.h"

RTTI_CLASS_DEFINITION(MovingPlatform, StandardAllocator)

Json MovingPlatform::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mMoveExtents);
	JSON_SAVE(json, mMoveSpeed);
	JSON_SAVE(json, mStartPosition);

	return json;
}

void MovingPlatform::Deserialize(const Json& inJson)
{
	JSON_TRY_LOAD(inJson, mMoveExtents);
	JSON_TRY_LOAD(inJson, mMoveSpeed);
	JSON_TRY_LOAD(inJson, mStartPosition);

	Base::Deserialize(inJson);
}

MovingPlatform::MovingPlatform(const ConstructParameters& inConstructParameters)
	: Base(inConstructParameters)
{
	TextureRenderComponent::ConstructParameters texture_render_component_parameters;
	texture_render_component_parameters.mTexture = gResourceManager.GetResource<TextureResource>("Assets/top.jpg");
	AddComponent<TextureRenderComponent>(texture_render_component_parameters);

	LoopOverComponents<CollisionComponent>([this](CollisionComponent& inCollisionComponent)
	{
		CollisionObjectWrapper collision_object = inCollisionComponent.GetCollisionObject();
		collision_object->SetType(CollisionObject::EType::Dynamic);
	});
}

void MovingPlatform::BeginPlay()
{
	Base::BeginPlay();

	if (mStartPosition.x == 0.f && mStartPosition.y == 0.f)
		mStartPosition = GetPosition();
}

void MovingPlatform::Update(float inDeltaTime)
{
	PROFILE_SCOPE(MovingPlatform::Update)

	Base::Update(inDeltaTime);

	fm::vec2 old_position = GetPosition();
	fm::vec2 position = old_position;

	position += mMoveSpeed * inDeltaTime;

	if (position.x > mStartPosition.x + mMoveExtents.x)
	{
		position.x = mStartPosition.x + mMoveExtents.x;
		mMoveSpeed.x *= -1.f;
	}
	else if (position.x < mStartPosition.x - mMoveExtents.x)
	{
		position.x = mStartPosition.x - mMoveExtents.x;
		mMoveSpeed.x *= -1.f;
	}

	if (position.y > mStartPosition.y + mMoveExtents.y)
	{
		position.y = mStartPosition.y + mMoveExtents.y;
		mMoveSpeed.y *= -1.f;
	}
	else if (position.y < mStartPosition.y - mMoveExtents.y)
	{
		position.y = mStartPosition.y - mMoveExtents.y;
		mMoveSpeed.y *= -1.f;
	}

	fm::Transform2D transform = GetTransform();
	transform.position = position;
	const Array<CollisionData>& collision_data = GetWorld()->GetCollisionWorld()->CheckCollisions(transform);
	for (const CollisionData& data : collision_data)
	{
		bool self = false;
		LoopOverComponents<CollisionComponent>([data, &self](CollisionComponent& inCollisionComponent)
		{
				if (data.mHandle == inCollisionComponent.GetCollisionObject()->GetHandle())
				self = true;
		});
		if (self)
			continue;

		if (data.mHandle.IsValid())
		{
			WeakRef<Entity> entity_weakref = GetWorld()->GetCollisionWorld()->GetCollisionObject(data.mHandle)->GetEntity();

			if (!entity_weakref.IsAlive())
				continue;

			Ref<Entity> entity = entity_weakref.Get();

			CollisionActor* actor = entity.Cast<CollisionActor>();
			if (actor != nullptr)
			{
				fm::vec2 actor_position = actor->GetPosition();
				actor_position += (position - old_position);
				actor->MoveTo(actor_position);
			}
		}
	}

	MoveTo(transform.position);
}
