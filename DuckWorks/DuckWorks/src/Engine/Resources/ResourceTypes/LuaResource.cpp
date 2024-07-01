#include "Precomp.h"
#include "Engine/Resources/ResourceTypes/LuaResource.h"

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
}

void LuaResource::RunScript(sol::state& inLua) const
{
	inLua.script(mScript);
}
