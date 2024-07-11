#pragma once
#include "Engine/Events/EventManager.h"
#include "Engine/Renderer/Camera.h"

#ifdef _DEBUG

class DebugCamera : public Camera
{
	RTTI_CLASS(DebugCamera, Camera, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	DebugCamera(const ConstructParameters& inParameters = {});

	virtual void Update(float inDeltaTime) override;

	void OnMouseMove(const EventManager::EventData& inData);
	void OnMouseWheel(const EventManager::EventData& inData);

private:
	Array<SharedPtr<EventManager::EventFunction>> mEventFunctions;
};

#endif
