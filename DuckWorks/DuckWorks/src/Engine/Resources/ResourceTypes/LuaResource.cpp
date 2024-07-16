#include "Precomp.h"
#include "Engine/Resources/ResourceTypes/LuaResource.h"

// Engine includes
#include "Engine/Resources/ResourceManager.h"

// External includes
#include "External/sol/sol.hpp"

// Std includes
#include <fstream>

RTTI_CLASS_DEFINITION(LuaResource, ClassAllocator)

RTTI_EMPTY_SERIALIZE_DEFINITION(LuaResource)

void LuaResource::LoadFromFile(const String& inFile)
{
	BaseResource::LoadFromFile(inFile);

	mScript = sReadFile(inFile);
	mScript.shrink_to_fit();
}

uint64 LuaResource::GetMemorySize() const
{
	return sizeof(LuaResource) + sizeof(String) + mScript.size();
}

void LuaResource::RunScript(sol::state& inLua) const
{
	inLua.script(mScript);
}

void from_json(const Json& inJson, SharedPtr<LuaResource>& outVariable)
{
	outVariable = gResourceManager.GetResource<LuaResource>(inJson["mFile"]);
	outVariable->Deserialize(inJson);
}
