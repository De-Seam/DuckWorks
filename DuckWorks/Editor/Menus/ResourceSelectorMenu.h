#pragma once
#include <Editor/Menus/EditorMenu.h>

class Resource;

class ResourceSelectorMenu : public EditorMenu
{
	RTTI_CLASS(ResourceSelectorMenu, EditorMenu)
public:
	ResourceSelectorMenu(EditorMenu& inParentMenu, const DC::RTTI& inResourceRTTI, DC::Ref<Resource>& ioResourceToSet) :
		Base(inParentMenu.GetEditorApp()),
		mParentMenu(&inParentMenu),
		mResourceRTTI(&inResourceRTTI),
		mResourceToSet(&ioResourceToSet) {}

	virtual void Update(float inDeltaTime) override;

private:
	EditorMenu* mParentMenu = nullptr;
	const DC::RTTI* mResourceRTTI = nullptr;
	DC::Ref<Resource>* mResourceToSet = nullptr;
};
