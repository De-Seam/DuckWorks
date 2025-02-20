#include <Editor/Menus/EditorMenu.h>

DC::Json EditorMenu::ToJson() const
{
	DC::Json json;
	json["mIsOpen"] = mIsOpen;
	return json;
}

void EditorMenu::FromJson(const DC::Json& inJson)
{
	if (auto it = inJson.find("mIsOpen"); it != inJson.end())
		mIsOpen = it.value();
}

void EditorMenu::Update(float inDeltaTime)
{
	if (!mIsOpen)
		return;

	for (EditorMenu* child : mChildMenus)
		child->Update(inDeltaTime);
}

void EditorMenu::AddChild(DC::Ref<EditorMenu> inChildMenu)
{
	mChildMenus.Add(Move(inChildMenu));
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
