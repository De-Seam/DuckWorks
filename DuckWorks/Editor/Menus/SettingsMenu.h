#pragma once
#include <Editor/Menus/EditorMenu.h>

class SettingsMenu : public EditorMenu
{
	RTTI_CLASS(SettingsMenu, EditorMenu)
public:
	SettingsMenu(EditorApp& inEditorApp) : Base(inEditorApp) {}

	virtual void Update(float inDeltaTime) override;
};