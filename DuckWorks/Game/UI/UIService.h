#pragma once
#include <DuckCore/Containers/Array.h>

#include <Game/UI/UIElement.h>
#include <Game/World/Service.h>

class WorldUpdateHandle;

class UIService : public Service
{
	RTTI_CLASS(UIService, Service)
public:
	UIService(World& inWorld);
	virtual ~UIService() override;

	void AddUIElement(DC::Ref<UIElement> inUIElement);

private:
	void Update(float inDeltaTime);

	DC::Ref<WorldUpdateHandle> mWorldUpdateHandle;
	DC::Array<DC::Ref<UIElement>> mUIElements;
};
