#include "Precomp.h"
#include "Engine/Debug/DebugCamera.h"

// Engine includes
#include "Engine/Debug/DebugUIWindowManager.h"
#include "Engine/Events/EventManager.h"
#include "Engine/Renderer/Renderer.h"

RTTI_CLASS_DEFINITION(DebugCamera, StandardAllocator)

RTTI_EMPTY_SERIALIZE_DEFINITION(DebugCamera)

DebugCamera::DebugCamera(const ConstructParameters& inParameters)
	: Base(inParameters)
{
	SetZoomSpeed(30.0f);

	{
		EventManager::EventFunction event_function;
		event_function.mEventType = EventType::MouseMove;
		event_function.mFunctionPtr = [this](const EventManager::EventData& inData) { OnMouseMove(inData); };
		mEventFunctions.emplace_back(gEventManager.AddEventFunction(event_function));
	}

	{
		EventManager::EventFunction event_function;
		event_function.mEventType = EventType::MouseWheel;
		event_function.mFunctionPtr = [this](const EventManager::EventData& inData) { OnMouseWheel(inData); };
		mEventFunctions.emplace_back(gEventManager.AddEventFunction(event_function));
	}
}

void DebugCamera::Update(float inDeltaTime)
{
	Camera::Update(inDeltaTime);
}

void DebugCamera::OnMouseMove(const EventManager::EventData& inData)
{
	if (gDebugUIWindowManager.WantsMouseCapture())
		return;

	if (gEventManager.IsMouseButtonDown(MouseButton::Middle))
	{
		Vec2 mouse_delta = inData.mMouseMove.mDelta;

		Vec2 modifier = Vec2(1.0f) / (GetSizeInverse() * Vec2(gRenderer.GetWindowSize())) / GetZoom();

		SnapPosition(GetTargetPosition() - mouse_delta * modifier);
	}
}

void DebugCamera::OnMouseWheel(const EventManager::EventData& inData)
{
	if (gDebugUIWindowManager.WantsMouseCapture())
		return;

	SetZoom(GetZoom() + inData.mMouseWheel.mDelta * 0.1f);
}
