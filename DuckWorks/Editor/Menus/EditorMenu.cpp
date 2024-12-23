#include <Editor/Menus/EditorMenu.h>

void EditorMenu::Update(float inDeltaTime)
{
	if (!mIsOpen)
		return;

	for (EditorMenu* child : mChildMenus)
		child->Update(inDeltaTime);
}

void EditorMenu::AddChild(DC::Ref<EditorMenu> inChildMenu)
{
	mChildMenus.Add(gMove(inChildMenu));
}

void EditorMenu::RemoveChild(EditorMenu& inChildMenu)
{
	const int index = mChildMenus.Find(&inChildMenu);
	if (index == -1)
	{
		gAssert(false);
		return;
	}
	mChildMenus.Remove(index);
}
