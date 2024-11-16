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
	outClass = Resource::sFindResourceInResourceManager<inClassName>(inJson["GUID"]); \
	if (outClass != nullptr) \
	{ \
		outClass = new inClassName; \
		outClass->SetGUID(inJson["GUID"]); \
	} \
	outClass->Deserialize(inJson); \
}

// Resource is a shared object
class Resource : public Object
{
	RTTI_CLASS(Resource, Object)
public:
	template<typename taType>
	static taType* sFindResourceInResourceManager(const DC::GUID& inGUID);

private:
	static Resource* sFindResourceInResourceManagerInternal(const DC::GUID& inGUID);
};

template<typename taType>
taType* Resource::sFindResourceInResourceManager(const DC::GUID& inGUID)
{
	Resource* resource = sFindResourceInResourceManagerInternal(inGUID);
	gAssert(resource == nullptr || resource->IsA<taType>(), "Loaded resource is of the wrong type! GUID collision?");
	return reinterpret_cast<taType*>(resource);
}
