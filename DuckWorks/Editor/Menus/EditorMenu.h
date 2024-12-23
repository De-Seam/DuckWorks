#pragma once
#include <DuckCore/Containers/Array.h>
#include <Engine/Objects/Object.h>

class EditorMenu : public Object
{
	RTTI_CLASS(EditorMenu, Object)
public:
	virtual void Update(float inDeltaTime);

	void AddChild(DC::Ref<EditorMenu> inChildMenu);
	void RemoveChild(EditorMenu& inChildMenu);

protected:
	bool mIsOpen = false;

private:
	DC::Array<DC::Ref<EditorMenu>> mChildMenus;
};
