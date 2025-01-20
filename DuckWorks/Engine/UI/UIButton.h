#pragma once
#include <Engine/UI/UIElement.h>

class UIButton : public UIElement
{
	RTTI_CLASS(UIButton, UIElement)
public:
	virtual void Update(float inDeltaTime) override;
};
