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

	CollisionComponent::ConstructParameters collision_component_params;
	collision_component_params.mType = CollisionObject::EType::Dynamic;
	AddComponent<CollisionComponent>(collision_component_params);
}

void MovingPlatform::BeginPlay()
{
	Base::BeginPlay();

	if (mStartPosition.mX == 0.f && mStartPosition.mY == 0.f)
		mStartPosition = GetPosition();
}

void MovingPlatform::Update(float inDeltaTime)
{
	PROFILE_SCOPE(MovingPlatform::Update)

	Base::Update(inDeltaTime);

	Vec2 old_position = GetPosition();
	Vec2 position = old_position;

	position += mMoveSpeed * inDeltaTime;

	if (position.mX > mStartPosition.mX + mMoveExtents.mX)
	{
		position.mX = mStartPosition.mX + mMoveExtents.mX;
		mMoveSpeed.mX *= -1.f;
	}
	else if (position.mX < mStartPosition.mX - mMoveExtents.mX)
	{
		position.mX = mStartPosition.mX - mMoveExtents.mX;
		mMoveSpeed.mX *= -1.f;
	}

	if (position.mY > mStartPosition.mY + mMoveExtents.mY)
	{
		position.mY = mStartPosition.mY + mMoveExtents.mY;
		mMoveSpeed.mY *= -1.f;
	}
	else if (position.mY < mStartPosition.mY - mMoveExtents.mY)
	{
		position.mY = mStartPosition.mY - mMoveExtents.mY;
		mMoveSpeed.mY *= -1.f;
	}

	Transform2D transform = GetTransform();
	transform.mPosition = position;
	const Array<CollisionData>& collision_data = GetWorld()->GetCollisionWorld()->CheckCollisions(transform);
	for (const CollisionData& data : collision_data)
	{
		bool self = false;
		LoopOverComponents<CollisionComponent>([data, &self](CollisionComponent& inCollisionComponent)
		{
			if (data.mHandle == inCollisionComponent.GetCollisionObject().GetHandle())
				self = true;
		});
		if (self)
			continue;

		if (data.mHandle.IsValid())
		{
			WeakRef<Entity> entity_weakref = GetWorld()->GetCollisionWorld()->GetCollisionObject(data.mHandle).GetEntity();

			if (!entity_weakref.IsAlive())
				continue;

			Ref<Entity> entity = entity_weakref.Get();

			bool is_dynamic = false;
			entity->LoopOverComponents<CollisionComponent>([&is_dynamic](CollisionComponent& inCollisionComponent)
			{
				if (inCollisionComponent.GetCollisionObject().GetType() == CollisionObject::EType::Dynamic)
					is_dynamic = true;
			});

			if (is_dynamic)
			{
				Vec2 entity_position = entity->GetPosition();
				entity_position += (position - old_position);
				entity->SetPosition(entity_position);
			}
		}
	}

	SetPosition(transform.mPosition);
}
