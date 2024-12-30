#pragma once
#include <Editor/Menus/EditorMenu.h>

class EntitySpawnerMenu : public EditorMenu
{
	RTTI_CLASS(EntitySpawnerMenu, EditorMenu)
public:
	EntitySpawnerMenu(EditorApp& inEditorApp) : Base(inEditorApp) {}

	virtual void Update(float inDeltaTime) override;
};