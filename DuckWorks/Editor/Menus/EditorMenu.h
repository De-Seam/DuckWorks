#pragma once
#include <DuckCore/Containers/Array.h>
#include <Engine/Objects/Object.h>

class EditorApp;

class EditorMenu : public Object
{
	RTTI_CLASS(EditorMenu, Object)
public:
	EditorMenu(EditorApp& inEditorApp) : mEditorApp(&inEditorApp) {}

	virtual DC::Json Serialize() const;
	virtual void Deserialize(const DC::Json& inJson);

	virtual void Update(float inDeltaTime);

	void AddChild(DC::Ref<EditorMenu> inChildMenu);
	void RemoveChild(EditorMenu& inChildMenu);

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
