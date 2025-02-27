#pragma once
#include <DuckCore/Events/Event.h>

// Before we send the update event.
class EnginePreUpdateEvent final : public DC::Event
{
	RTTI_CLASS(EnginePreUpdateEvent, Event)
};

// The update event for the engine.
class EngineUpdateEvent final : public DC::Event
{
	RTTI_CLASS(EngineUpdateEvent, Event)
public:
	explicit EngineUpdateEvent(float aDeltaTime) : mDeltaTime(aDeltaTime) {}

	float GetDeltaTime() const { return mDeltaTime; }

private:
	float mDeltaTime = 0.0f;
};

// After we call the update event.
class EnginePostUpdateEvent final : public DC::Event
{
	RTTI_CLASS(EnginePostUpdateEvent, Event)
};

// Before we call BeginFrame on the Renderer.
class EnginePreRenderEvent final : public DC::Event
{
	RTTI_CLASS(EnginePreRenderEvent, Event)
};

// After we call BeginFrame on the Renderer.
class EngineRenderEvent final : public DC::Event
{
	RTTI_CLASS(EngineRenderEvent, Event)
};

// After we render the game, but before we call EndFrame.
class EnginePostRenderEvent final : public DC::Event
{
	RTTI_CLASS(EnginePostRenderEvent, Event)
};