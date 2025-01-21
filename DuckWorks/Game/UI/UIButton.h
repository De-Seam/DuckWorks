#pragma once
#include <Game/UI/UIElement.h>

class UIButton : public UIElement
{
	RTTI_CLASS(UIButton, UIElement)
public:
	virtual void Update(float inDeltaTime) override;

	void SetRectangle(const DC::FRect2D& inRectangle) { mRectangle = inRectangle; }
	void SetColor(const DC::RGBA& inColor) { mColor = inColor; }
	void SetHoverColor(const DC::RGBA& inHoverColor) { mHoverColor = inHoverColor; }
	void SetPressedColor(const DC::RGBA& inPressedColor) { mPressedColor = inPressedColor; }

private:
	DC::FRect2D mRectangle = {0.f, 0.f, 120.f, 120.f};
	DC::RGBA mColor = DC::RGBA((uint8)255, (uint8)255, (uint8)255, (uint8)255);
	DC::RGBA mHoverColor = DC::RGBA((uint8)180, (uint8)180, (uint8)180, (uint8)255);
	DC::RGBA mPressedColor = DC::RGBA((uint8)120, (uint8)120, (uint8)120, (uint8)255);
};
