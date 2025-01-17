#pragma once
#include <Editor/Menus/EditorMenu.h>

class ResourceSelectorMenu : public EditorMenu
{
	RTTI_CLASS(ResourceSelectorMenu, EditorMenu)
public:
	ResourceSelectorMenu(EditorApp& inEditorApp) : Base(inEditorApp) {}

	virtual void Update(float inDeltaTime) override;
private:

};
