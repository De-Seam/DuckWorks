#include "Precomp.h"
#include "Engine/Resources/ResourceTypes/LuaResource.h"

// Engine includes
#include "Engine/Resources/ResourceManager.h"

// External includes
#include "External/sol/sol.hpp"

// Std includes
#include <fstream>

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
	if (inJson.empty())
		return;

	outVariable = gResourceManager.GetResource<LuaResource>(inJson["mFile"]);
	outVariable->Deserialize(inJson);
}
