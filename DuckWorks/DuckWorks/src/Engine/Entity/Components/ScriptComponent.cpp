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

	JSON_SAVE(json, mUpdateScript);

	return json;
}

void ScriptComponent::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	if (inJson.contains("mUpdateScript"))
	{
		const Json& update_script_json = inJson["mUpdateScript"];
		if (update_script_json.empty())
			mUpdateScript = nullptr;

		const String& lua_update_file = update_script_json["mFile"];
		if (lua_update_file.empty())
		{
			mUpdateScript = nullptr;
		}
		else if (gResourceManager.FileExists(lua_update_file.c_str()) && gIsValidLuaExtension(lua_update_file))
		{
			mUpdateScript = gResourceManager.GetResource<LuaResource>(update_script_json["mFile"]);
		}
	}
	else
	{
		mUpdateScript = nullptr;
	}
}

ScriptComponent::ScriptComponent(const ConstructParameters& inConstructParameters)
	: Base(inConstructParameters), mUpdateScript(inConstructParameters.mUpdateScript) {}

void ScriptComponent::Update(float inDeltaTime)
{
	if (mUpdateScript == nullptr)
		return;

	sol::state& lua = gEngine.GetLua();
	lua["Entity"] = GetEntity();
	lua["deltaTime"] = inDeltaTime;
	mUpdateScript->RunScript(lua);
}
