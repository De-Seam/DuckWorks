#include "Precomp.h"
#include "Engine/UI/UIManager.h"

// Engine includes
#include "Engine/UI/UIWidget.h"

RTTI_CLASS_DEFINITION(UIManager, StandardAllocator)

RTTI_EMPTY_SERIALIZE_DEFINITION(UIManager)

UIManager gUIManager;

void UIManager::Update(float inDeltaTime)
{
	PROFILE_SCOPE(UIManager::Update)

	for (UIWidget* window : mRootWindows)
		window->Update(inDeltaTime);
}

void UIManager::Draw()
{
	PROFILE_SCOPE(UIManager::Draw)

	for (UIWidget* window : mRootWindows)
		window->Draw();
}
