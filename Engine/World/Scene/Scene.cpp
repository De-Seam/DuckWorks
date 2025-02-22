#include <Engine/World/Scene/Scene.h>

#include <DuckCore/Managers/Managers.h>

#include <Engine/Renderer/Renderer.h>

using namespace DC;

Scene::Scene(const GUID& aGUID, const Json& aJson) :
	Base(aGUID)
{
	
}

Ref<Scene> Scene::sNew()
{
	return new Scene(GUID::sCreate());
}

void Scene::Render()
{
	Renderer& renderer = Managers::sGet<Renderer>();

	if (mRenderTarget == nullptr)
		mRenderTarget = renderer.CreateRenderTarget(IVec2(cRenderTargetWidth, cRenderTargetHeight));

	Renderer::ScopedRenderTarget scoped_render_target(*mRenderTarget);

	renderer.Clear(RGBA::sWhite());
}

void Scene::AddEntity(Ref<Entity>& aEntity)
{
	gAssert(!mEntities.Contains(aEntity));
	mEntities.Add(aEntity);
	aEntity->OnAddedToScene(*this);
}

void Scene::RemoveEntity(Entity& aEntity)
{
	mEntities.SwapRemoveFirstIf([&aEntity](const Entity* aItem) { return aItem == &aEntity; });
	aEntity.OnRemovedFromScene(*this);
}

Scene::Scene(const GUID& aGUID) :
	Base(aGUID)
{}
