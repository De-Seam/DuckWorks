#include <Engine/Engine.h>
#include <Engine/UI/UIButton.h>

#include <Engine/Renderer/Renderer.h>

using namespace DC;

void UIButton::Update(float inDeltaTime)
{
	Renderer& renderer = gEngine->GetManager<Renderer>();
	renderer.DrawRectangle()

	Base::Update(inDeltaTime);
}
