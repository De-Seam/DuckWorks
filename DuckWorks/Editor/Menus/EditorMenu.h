#pragma once
#include <DuckCore/Containers/Array.h>

#include <Engine/Objects/Object.h>

class EditorApp;

class EditorMenu : public Object
{
	RTTI_CLASS(EditorMenu, Object)
public:
	EditorMenu(EditorApp& inEditorApp) : mEditorApp(&inEditorApp) {}

	virtual DC::Json ToJson() const;
	virtual void FromJson(const DC::Json& inJson);

	virtual void Update(float inDeltaTime);

	void AddChild(DC::Ref<EditorMenu> inChildMenu);
	void RemoveChild(EditorMenu& inChildMenu);
	template<typename taType>
	void RemoveChildrenOfType();

	void SetIsOpen(bool inIsOpen) { mIsOpen = inIsOpen; }
	bool GetIsOpen() const { return mIsOpen; }

	EditorApp& GetEditorApp() { return *mEditorApp; }
	const EditorApp& GetEditorApp() const { return *mEditorApp; }

protected:
	bool mIsOpen = false;

private:
	EditorApp* mEditorApp = nullptr;
	DC::Array<DC::Ref<EditorMenu>> mChildMenus;
};

template <typename taType>
void EditorMenu::RemoveChildrenOfType()
{
	mChildMenus.SwapRemoveIf([](const EditorMenu* inMenu)
	{
		return inMenu->IsA<taType>();
	});
}
