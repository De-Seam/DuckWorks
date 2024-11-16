#pragma once
// Engine includes
#include <Engine/Objects/Object.h>

// Allow automatic serialization and deserialization using Json["X"] = Y; and Y = Json["X"];
#define RESOURCE_AUTO_JSON(inClassName) \
inline void to_json(Json& outJson, const inClassName* inClass) \
{ \
	outJson = inClass->Serialize(); \
} \
\
inline void from_json(const Json& inJson, inClassName*& outClass) \
{ \
	outClass = gEngine->GetManager<ResourceManager>().GetResource<inClassName>(inJson["mFile"]); \
	outClass->Deserialize(inJson); \
}

// Resource is a shared object that is loaded from a file.
// Resource types should inherit from this base class.
// Resources are automatically loaded and unloaded by the ResourceManager.
class Resource : public Object
{
	RTTI_CLASS(Resource, Object)
public:
	// Load the resource
	virtual void Load() = 0;
	// Unload the resource
	virtual void Unload() = 0;

	virtual Json Serialize() const override;
	virtual void Deserialize(const Json& inJson) override;

	const DC::String& GetFile() const { return mFile; }

private:
	// Constructor is private to prevent direct instantiation. Only the ResourceManager may create resources.
	Resource(const DC::String& inFile) : mFile(inFile) {}

	DC::String mFile = "";

	friend class ResourceManager;
};
