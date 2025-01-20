#pragma once
#include <DuckCore/Containers/Array.h>

#include <Engine/Objects/Object.h>

class UIElement : public Object
{
	RTTI_CLASS(UIWidget, Object)
public:
	virtual void Update(float inDeltaTime);

	void AddChild(UIElement& inChild);
	void RemoveChild(UIElement& inChild);

private:
	DC::Array<DC::Ref<UIElement>> mChildren;
};