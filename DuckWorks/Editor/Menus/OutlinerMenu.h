#pragma once
#include <Editor/Menus/EditorMenu.h>
#include <Game/Entity/Entity.h>

class OutlinerMenu : public EditorMenu
{
	RTTI_CLASS(Outlinermenu, EditorMenu)
public:
	OutlinerMenu(EditorApp& inEditorApp) : Base(inEditorApp) {}

	virtual void Update(float inDeltaTime) override;

private:
	void DrawEntity(Entity& inEntity);
};
