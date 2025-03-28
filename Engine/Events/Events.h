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
