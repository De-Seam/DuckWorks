#include <Engine/UI/UIElement.h>

using namespace DC;

void UIElement::Update(float inDeltaTime) 
{
	for (UIElement* child : mChildren)
		child->Update(inDeltaTime);
}

void UIElement::AddChild(UIElement& inChild) 
{
	gAssert(!mChildren.Contains(&inChild));
	mChildren.Add(&inChild);
}

void UIElement::RemoveChild(UIElement& inChild) 
{
	const int index = mChildren.Find(&inChild);
	gAssert(index != -1, "Child not found");

	if (index != -1)
		mChildren.Remove(index);
}
