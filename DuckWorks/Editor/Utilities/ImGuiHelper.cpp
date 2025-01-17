#include <Editor/Utilities/ImGuiHelper.h>

using namespace DC;

void ImGuiHelpers::sDrawResourceLink(const char* inLabel, Ref<Resource>& ioResource)
{
	String guid_string = ioResource->GetGUID().ToString();
	if (ImGui::Button(*guid_string))
	{
		
	}
}
