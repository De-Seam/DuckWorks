#pragma once
// Engine includes
#include <DuckCore/Containers/File.h>
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

private:
	friend class ResourceManager;
};
