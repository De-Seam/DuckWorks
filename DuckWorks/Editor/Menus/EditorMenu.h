#pragma once
#include <DuckCore/Containers/Array.h>
#include <Engine/Objects/Object.h>

class EditorMenu : public Object
{
	RTTI_CLASS(EditorMenu, Object)
public:
	virtual DC::Json Serialize() const;
	virtual void Deserialize(const DC::Json& inJson);

	virtual void Update(float inDeltaTime);

	void AddChild(DC::Ref<EditorMenu> inChildMenu);
	void RemoveChild(EditorMenu& inChildMenu);

	void SetIsOpen(bool inIsOpen) { mIsOpen = inIsOpen; }
	bool GetIsOpen() const { return mIsOpen; }

protected:
	bool mIsOpen = false;

private:
	DC::Array<DC::Ref<EditorMenu>> mChildMenus;
};
