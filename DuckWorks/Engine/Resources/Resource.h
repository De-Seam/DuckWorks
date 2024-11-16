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
	outClass = reinterpret_cast<inClassName*>(Resource::sGetResourceInResourceManager<inClassName>(inJson["GUID"])); \
	outClass->Deserialize(inJson); \
}

#define FILE_RESOURCE_AUTO_JSON(inClassName) \
inline void to_json(Json& outJson, const inClassName* inClass) \
{ \
	outJson = inClass->Serialize(); \
} \
\
inline void from_json(const Json& inJson, inClassName*& outClass) \
{ \
	outClass = reinterpret_cast<inClassName*>(FileResource::sGetResourceInResourceManager<inClassName>(inJson["File"])); \
	outClass->Deserialize(inJson); \
}

// Resource is a shared object
class Resource : public Object
{
	RTTI_CLASS(Resource, Object)
public:
	Resource(const DC::GUID& inGUID) : Object(inGUID) {}

	static Resource* sGetResourceInResourceManager(const DC::GUID& inGUID);
private:
};

// FileResource is a Resource loaded from a file
class FileResource : public Resource
{
	RTTI_CLASS(FileResource, Resource)
public:
	FileResource(const DC::GUID& inGUID, const DC::String& inFile) : Resource(inGUID), mFile(inFile) { (void)inFile; }

	static Resource* sGetResourceInResourceManager(const DC::String& inFile);

private:
	DC::String mFile = "";
};

