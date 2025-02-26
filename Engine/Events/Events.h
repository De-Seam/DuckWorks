#pragma once
#include <DuckCore/Events/Event.h>

class EngineUpdateEvent final : public DC::Event
{
	RTTI_CLASS(EngineUpdateEvent, Event)
public:
	explicit EngineUpdateEvent(float aDeltaTime) : mDeltaTime(aDeltaTime) {}

	float GetDeltaTime() const { return mDeltaTime; }

private:
	float mDeltaTime = 0.0f;
};