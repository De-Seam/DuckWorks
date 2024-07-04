#pragma once
#include <External/phmap/phmap.h>
#include <External/SDL/SDL_events.h>

#include <functional>
#include <vector>

struct SDLEventFunction
{
	SDLEventFunction() = default;

	SDLEventFunction(SDL_EventType inEventType, const std::function<void(const SDL_Event&)>& inFunctionPtr)
		: mEventType(inEventType), mFunctionPtr(inFunctionPtr) {}

	SDL_EventType mEventType = SDL_FIRSTEVENT;
	bool mAllEvents = false; ///< Overrides mEventType
	std::function<void(const SDL_Event&)> mFunctionPtr = nullptr;
};

class SDLEventManager : public Manager
{
	RTTI_CLASS(SDLEventManager, Manager, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	SDLEventManager(const ConstructParameters& inParameters = {}) : Base(inParameters) { mRequiresUpdate = true; }

	// @a inEventFunction is owned by shared ptr owned by the class that calls this function
	// @a inAllEvents is used to add the event function to all events instead of the mEventType
	[[nodiscard]] std::shared_ptr<SDLEventFunction> AddEventFunction(const SDLEventFunction& inEventFunction);

	// This function can be used for an event function that will never be destroyed for the lifetime of the program
	void AddPersistentEventFunction(const SDLEventFunction& inEventFunction);

	virtual void Update(float inDeltaTime) override;

private:
	void UpdateCallEventFunctions(std::vector<std::weak_ptr<SDLEventFunction>>& inEventsFunctions, const SDL_Event& inEvent);

	phmap::flat_hash_map<SDL_EventType, std::vector<std::weak_ptr<SDLEventFunction>>> mEventsFunctions;
	std::vector<WeakPtr<SDLEventFunction>> mEventsFunctionAll;
	std::vector<SharedPtr<SDLEventFunction>> mPersistentEventsFunctions;
};

extern SDLEventManager gSDLEventManager;
