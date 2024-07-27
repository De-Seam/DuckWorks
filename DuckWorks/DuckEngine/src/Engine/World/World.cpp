#include <Engine/Precomp.h>
#include <Engine/World/World.h>

// Engine includes
#include <Engine/Entity/Entity.h>

void World::Update(float inDeltaTime) 
{
	PROFILE_SCOPE(World::Update)

	mIsUpdatingEntities = true;

	for (Entity* entity : mEntities)
		entity->Update(inDeltaTime);

	mIsUpdatingEntities = false;

	for (const Ref<Entity>& entity : mEntitiesToAdd)
		AddEntity(entity);

	for (const Ref<Entity>& entity : mEntitiesToRemove)
		RemoveEntity(entity);
}

void World::Render() 
{
	PROFILE_SCOPE(World::Render)

	//for (Entity* entity : mEntities)
	//	entity->Render();
}

void World::AddEntity(const Ref<Entity>& inEntity) 
{
	if (mIsUpdatingEntities)
	{
		mEntitiesToAdd.push_back(inEntity);
		return;
	}
	mEntities.push_back(inEntity);
}

void World::RemoveEntity(const Ref<Entity>& inEntity) 
{
	if (mIsUpdatingEntities)
	{
		mEntitiesToRemove.push_back(inEntity);
		return;
	}
	Array<Ref<Entity>>::iterator it = std::ranges::find(mEntities.begin(), mEntities.end(), inEntity);
	gSwap(*it, mEntities.back());
	mEntities.pop_back();
}
