#include <Game/UI/UIService.h>

#include <Game/World/World.h>

using namespace DC;

UIService::UIService(World& inWorld) :
	Base(inWorld)
{
	mWorldUpdateHandle = GetWorld().RegisterUpdateCallback([this](float inDeltaTime){ Update(inDeltaTime); });
}

UIService::~UIService() = default;

void UIService::AddUIElement(Ref<UIElement> inUIElement) 
{
	mUIElements.Add(gMove(inUIElement));
}

void UIService::Update(float inDeltaTime)
{
	for (UIElement* element : mUIElements)
		element->Update(inDeltaTime);
}
