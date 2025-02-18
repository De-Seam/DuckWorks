#include <Editor/Menus/ResourceSelectorMenu.h>
#include <Engine/Engine.h>
#include <Engine/Resources/ResourceManager.h>
#include <imgui/imgui.h>

using namespace DC;

void ResourceSelectorMenu::Update(float inDeltaTime)
{
	if (!mIsOpen)
		return;

	ImGui::Begin("Select Resource", &mIsOpen);

	static Array<const ResourceManager::ResourceLinkInfo*> sCachedResourceLinkInfos;
	sCachedResourceLinkInfos.Clear();
	Managers::sGet<ResourceManager>().GetResourceLinkInfosOfType(mResourceRTTI->GetClassName(), sCachedResourceLinkInfos);

	bool should_close = false;
	for (const ResourceManager::ResourceLinkInfo* resource_link_info : sCachedResourceLinkInfos)
	{
		String guid_string = resource_link_info->mGUID.ToString();
		if (ImGui::Button(*guid_string))
		{
			*mResourceToSet = Managers::sGet<ResourceManager>().Find<Resource>(resource_link_info->mGUID);
			should_close = true;
			break;
		}
	}

	ImGui::End();

	Base::Update(inDeltaTime);

	if (should_close)
		mParentMenu->RemoveChild(*this);
}