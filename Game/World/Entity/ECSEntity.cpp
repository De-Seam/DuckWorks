#include <Game/World/Scene/Scene.h>

using namespace DC;

ECSEntity::ECSEntity(Scene& aScene, entt::entity aEntityHandle) :
	mScene(&aScene),
	mEntityHandle(aEntityHandle)
{}

entt::registry& ECSEntity::GetRegistry()
{
	gAssert(mScene != nullptr);
	return mScene->GetRegistry();
}

const entt::registry& ECSEntity::GetRegistry() const
{
	gAssert(mScene != nullptr);
	return mScene->GetRegistry();
}
