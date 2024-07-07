#include "Precomp.h"
#include "Engine/Debug/DebugCamera.h"

// Engine includes
#include "Engine/Events/EventManager.h"

// External includes
#include "Engine/Renderer/Renderer.h"

#include "External/imgui/imgui.h"

RTTI_CLASS_DEFINITION(DebugCamera, StandardAllocator)

RTTI_EMPTY_SERIALIZE_DEFINITION(DebugCamera)

DebugCamera::DebugCamera(const ConstructParameters& inParameters)
	: Base(inParameters)
{
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
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
		return;

	if (gEventManager.IsMouseButtonDown(MouseButton::Middle))
	{
		Vec2 mouse_delta = inData.mMouseMove.mDelta;

		Vec2 modifier = Vec2(1.0f) / (GetSizeInverse() * Vec2(gRenderer.GetWindowSize())) / GetZoom();

		SnapPosition(GetTargetPosition() - mouse_delta * modifier);
		gLog("%v2", mouse_delta);
	}
}

void DebugCamera::OnMouseWheel(const EventManager::EventData& inData)
{
	SetZoom(GetZoom() + inData.mMouseWheel.mDelta * 0.1f);
}
