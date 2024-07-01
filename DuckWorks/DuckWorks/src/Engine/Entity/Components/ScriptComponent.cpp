#include "Precomp.h"
#include "Engine/Entity/Components/ScriptComponent.h"

// Engine includes
#include "Engine/Engine/Engine.h"
#include "Engine/Entity/Entity.h"
#include "Engine/Resources/ResourceManager.h"
#include "Engine/Resources/ResourceTypes/LuaResource.h"

// ScriptComponent
RTTI_CLASS_DEFINITION(ScriptComponent, ClassAllocator)

Json ScriptComponent::Serialize()
{
	Json json = Base::Serialize();

	json["mFileName"] = mUpdateScript->GetFileName();

	return json;
}

void ScriptComponent::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	if (inJson.contains("mFileName"))
	{
		const String& lua_update_file = inJson["mFileName"];
		if (lua_update_file.empty())
		{
			mUpdateScript = nullptr;
		}
		else if (gEngine.FileExists(lua_update_file.c_str()) && gIsValidLuaExtension(lua_update_file))
		{
			mUpdateScript = gResourceManager.GetResource<LuaResource>(inJson["mFileName"]);
		}
	}
}

ScriptComponent::ScriptComponent(const ConstructParameters& inConstructParameters)
	: Base(inConstructParameters), mUpdateScript(inConstructParameters.mUpdateScript) {}

void ScriptComponent::Update(float inDeltaTime)
{
	sol::state& lua = gEngine.GetLua();
	lua["Entity"] = GetEntity();
	lua["deltaTime"] = inDeltaTime;
	mUpdateScript->RunScript(lua);
}
