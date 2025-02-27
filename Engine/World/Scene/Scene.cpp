#include <Engine/World/Scene/Scene.h>

#include <DuckCore/Managers/Managers.h>

#include <Engine/Renderer/Renderer.h>

using namespace DC;

Scene::Scene(const GUID& aGUID) :
	Base(aGUID)
{}

Scene::Scene(const Json& aJson)
{
	SetGUID(aJson["mGUID"]);
	for (const Json& entity_json : aJson["Entities"])
	{
		Ref<Entity> entity = new Entity(*this, entity_json);
		AddEntity(entity);
	}
}


void Scene::Update(float aDeltaTime)
{
	
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
}

void Scene::RemoveEntity(Entity& aEntity)
{
	mEntities.SwapRemoveFirstIf([&aEntity](const Entity* aItem) { return aItem == &aEntity; });
}

Json Scene::ToJson() const
{
	Json json = Base::ToJson();
	Json& json_entities = json["Entities"];

	for (const Ref<Entity>& entity : mEntities)
		json_entities.push_back(entity->ToJson());

	return json;
}
