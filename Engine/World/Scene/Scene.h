#pragma once
#include <DuckCore/Containers/Array.h>

#include <Engine/Objects/Object.h>
#include <Engine/Renderer/RenderTarget.h>
#include <Engine/World/Entity/Entity.h>

#include <External/entt/entity/registry.hpp>

#include <SDL/SDL_render.h>

class Scene : public Object
{
	RTTI_CLASS(Scene, Object)
public:
	static constexpr int cRenderTargetWidth = 1920;
	static constexpr int cRenderTargetHeight = 1080;

	Scene(const DC::GUID& aGUID, const DC::Json& aJson); // Load a Scene from a JSON file.
	static DC::Ref<Scene> sNew(); // Create a new Scene.

	void Render();

	void AddEntity(DC::Ref<Entity>& aEntity);
	void RemoveEntity(Entity& aEntity);

	entt::registry& GetRegistry() { return mRegistry; }
	RenderTarget* GetRenderTarget() { return mRenderTarget; }

private:
	explicit Scene(const DC::GUID& aGUID); // Create a new Scene

	DC::Array<DC::Ref<Entity>> mEntities;
	entt::registry mRegistry;

	DC::Ref<RenderTarget> mRenderTarget;
};
