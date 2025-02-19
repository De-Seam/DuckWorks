#pragma once
// Engine includes
#include <Engine/Engine.h>
#include <Engine/Objects/Object.h>

// File, loaded by FileManager
// Resource can just be data, identified by UUID (saved to disk)
// ResourceManager->GetResource(UUID) -> Resource::Load(UUID)
// -> FileManager->GetFile(File)

// Resource is a shared object that is loaded from a file.
// Resource types should inherit from this base class.
// Resources are automatically loaded and unloaded by the ResourceManager.
class Resource : public Object
{
	RTTI_CLASS(Resource, Object)
public:
	Resource(const DC::Json& inJson);
	virtual ~Resource() override = default;

	virtual DC::Json ToJson() const;

	virtual void UpdateImGui(); // Update for ImGui, in the editor. This is already inside a window.

private:
	DC::String mName; // (Display) name of the resource.

	friend class ResourceManager;
};
