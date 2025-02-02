#include "Precomp.h"
#include "Engine/Events/SDLEventManager.h"

#include "Engine/Renderer/Renderer.h"
#include "External/imgui/imgui_impl_sdl2.h"

RTTI_CLASS_DEFINITION(SDLEventManager, StandardAllocator)

RTTI_EMPTY_SERIALIZE_DEFINITION(SDLEventManager)

SDLEventManager gSDLEventManager;

SharedPtr<SDLEventFunction> SDLEventManager::AddEventFunction(const SDLEventFunction& inEventFunction)
{
	std::shared_ptr<SDLEventFunction> event_function = std::make_shared<SDLEventFunction>(inEventFunction);
	if (inEventFunction.mAllEvents)
	{
		mEventsFunctionAll.push_back(event_function);
	}
	else
	{
		mEventsFunctions[inEventFunction.mEventType].push_back(event_function);
	}
	return event_function;
}

void SDLEventManager::AddPersistentEventFunction(const SDLEventFunction& inEventFunction)
{
	mPersistentEventsFunctions.push_back(AddEventFunction(inEventFunction));
}

void SDLEventManager::Update([[maybe_unused]] float inDeltaTime)
{
	OPTICK_EVENT("SDLEventManager::Update");

	SDL_Event event;
	while (SDL_PollEvent(&event) != 0)
	{
		//IMGUI(ImGui_ImplSDL2_ProcessEvent(&event));

		UpdateCallEventFunctions(mEventsFunctionAll, event);

		UpdateCallEventFunctions(mEventsFunctions[static_cast<SDL_EventType>(event.type)], event);
	}
}

void SDLEventManager::UpdateCallEventFunctions(std::vector<std::weak_ptr<SDLEventFunction>>& inEventsFunctions, const SDL_Event& inEvent)
{
	for (std::weak_ptr<SDLEventFunction>& event_function : inEventsFunctions)
	{
		if (!event_function.expired())
		{
			std::shared_ptr<SDLEventFunction> event_function_ptr = event_function.lock();
			event_function_ptr->mFunctionPtr(inEvent);
		}
	}
}
