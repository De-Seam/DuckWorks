#include <Engine/Resources/Resource.h>

// Engine includes
#include <Engine/Engine.h>
#include <Engine/Files/FileManager.h>
#include <Engine/Objects/ObjectManager.h>
#include <Engine/Resources/ResourceManager.h>

#include <imgui/imgui.h>

using namespace DC;

Resource::Resource(const Json& inJson)
{
	const GUID& guid = inJson["mGUID"];
	SetGUID(guid);
}

Json Resource::ToJson() const
{
	Json json;
	json["ClassName"] = GetRTTI().GetClassName();
	json["mGUID"] = GetGUID();
	return json;
}

void Resource::UpdateImGui()
{
	char name_buffer[256] = {};
	memcpy(name_buffer, *mName, mName.Length());
	if (ImGui::InputText("Name", name_buffer, 256))
	{
		mName = name_buffer;
	}
}
