#pragma once
#include <External/SDL/SDL_events.h>
#include <External/phmap/phmap.h>

#include <vector>
#include <functional>

struct SDLEventFunction
{
	SDLEventFunction() = default;

	SDLEventFunction(SDL_EventType inEventType, const std::function<void(const SDL_Event&)>& inFunctionPtr)
		: mEventType(inEventType), mFunctionPtr(inFunctionPtr) {}

	SDL_EventType mEventType = SDL_FIRSTEVENT;
	std::function<void(const SDL_Event&)> mFunctionPtr = nullptr;
};

class SDLEventManager
{
public:
	// @a inEventFunction is owned by shared ptr owned by the class that calls this function
	// @a inAllEvents is used to add the event function to all events instead of the mEventType
	std::shared_ptr<SDLEventFunction> AddEventFunction(const SDLEventFunction& inEventFunction, bool inAllEvents = false);
	void Update();

private:
	void UpdateCallEventFunctions(std::vector<std::weak_ptr<SDLEventFunction>>& inEventsFunctions, const SDL_Event& inEvent);

	phmap::flat_hash_map<SDL_EventType, std::vector<std::weak_ptr<SDLEventFunction>>> mEventsFunctions;
	std::vector<std::weak_ptr<SDLEventFunction>> mEventsFunctionAll;
};

extern SDLEventManager gSDLEventManager;
