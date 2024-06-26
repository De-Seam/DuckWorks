#include "Precomp.h"
#include "Engine/Events/EventManager.h"

// Engine includes
#include "Engine/Events/SDLEventManager.h"

EventManager gEventManager;

void EventManager::Init()
{
	PROFILE_SCOPE(EventManager::Init)

	SetupSDLConversions();

	SDLEventFunction event_function;
	event_function.mEventType = SDL_KEYDOWN;
	event_function.mFunctionPtr = [this](const SDL_Event& inEvent) { this->OnKeyDown(inEvent); };
	gSDLEventManager.AddPersistentEventFunction(event_function);

	event_function.mEventType = SDL_KEYUP;
	event_function.mFunctionPtr = [this](const SDL_Event& inEvent) { this->OnKeyUp(inEvent); };
	gSDLEventManager.AddPersistentEventFunction(event_function);

	event_function.mEventType = SDL_MOUSEBUTTONDOWN;
	event_function.mFunctionPtr = [this](const SDL_Event& inEvent) { this->OnMouseDown(inEvent); };
	gSDLEventManager.AddPersistentEventFunction(event_function);

	event_function.mEventType = SDL_MOUSEBUTTONUP;
	event_function.mFunctionPtr = [this](const SDL_Event& inEvent) { this->OnMouseUp(inEvent); };
	gSDLEventManager.AddPersistentEventFunction(event_function);

	event_function.mEventType = SDL_MOUSEMOTION;
	event_function.mFunctionPtr = [this](const SDL_Event& inEvent) { this->OnMouseMove(inEvent); };
	gSDLEventManager.AddPersistentEventFunction(event_function);
}

void EventManager::SetupSDLConversions()
{
	mSDLKeycodeToKeyCode =
	{
		{SDLK_a, KeyCode::A},
		{SDLK_b, KeyCode::B},
		{SDLK_c, KeyCode::C},
		{SDLK_d, KeyCode::D},
		{SDLK_e, KeyCode::E},
		{SDLK_f, KeyCode::F},
		{SDLK_g, KeyCode::G},
		{SDLK_h, KeyCode::H},
		{SDLK_i, KeyCode::I},
		{SDLK_j, KeyCode::J},
		{SDLK_k, KeyCode::K},
		{SDLK_l, KeyCode::L},
		{SDLK_m, KeyCode::M},
		{SDLK_n, KeyCode::N},
		{SDLK_o, KeyCode::O},
		{SDLK_p, KeyCode::P},
		{SDLK_q, KeyCode::Q},
		{SDLK_r, KeyCode::R},
		{SDLK_s, KeyCode::S},
		{SDLK_t, KeyCode::T},
		{SDLK_u, KeyCode::U},
		{SDLK_v, KeyCode::V},
		{SDLK_w, KeyCode::W},
		{SDLK_x, KeyCode::X},
		{SDLK_y, KeyCode::Y},
		{SDLK_z, KeyCode::Z},
		{SDLK_0, KeyCode::Zero},
		{SDLK_1, KeyCode::One},
		{SDLK_2, KeyCode::Two},
		{SDLK_3, KeyCode::Three},
		{SDLK_4, KeyCode::Four},
		{SDLK_5, KeyCode::Five},
		{SDLK_6, KeyCode::Six},
		{SDLK_7, KeyCode::Seven},
		{SDLK_8, KeyCode::Eight},
		{SDLK_9, KeyCode::Nine},
		{SDLK_DELETE, KeyCode::Delete}
	};

	mSDLButtonToMouseButton =
	{
		{SDL_BUTTON_LEFT, MouseButton::Left},
		{SDL_BUTTON_RIGHT, MouseButton::Right},
		{SDL_BUTTON_MIDDLE, MouseButton::Middle},
		{SDL_BUTTON_X1, MouseButton::X1},
		{SDL_BUTTON_X2, MouseButton::X2}
	};
}

void EventManager::Update(float inDeltaTime)
{
	(void)inDeltaTime;
}

SharedPtr<EventManager::EventFunction> EventManager::AddEventFunction(const EventFunction& inFunction)
{
	SharedPtr<EventFunction> event_function = std::make_shared<EventFunction>(inFunction);
	mEventFunctions[inFunction.mEventType].push_back(event_function);
	return event_function;
}

void EventManager::AddPersistentEventFunction(const EventFunction& inFunction)
{
	SharedPtr<EventFunction> event_function = AddEventFunction(inFunction);
	mPersistentEventFunctions.push_back(event_function);
}

bool EventManager::IsKeyDown(KeyCode inKeyCode)
{
	return mKeyCodeDown[SCast<uint16>(inKeyCode)];
}

bool EventManager::IsMouseButtonDown(MouseButton inMouseButton)
{
	return mMouseButtonDown[SCast<uint16>(inMouseButton)];
}

fm::vec2 EventManager::GetOldMousePosition()
{
	return mOldMousePosition;
}

fm::vec2 EventManager::GetMousePosition()
{
	return mMousePosition;
}

void EventManager::OnKeyDown(const SDL_Event& inEvent)
{
	KeyCode key_code = mSDLKeycodeToKeyCode[inEvent.key.keysym.sym];

	bool was_down = mKeyCodeDown[SCast<uint16>(key_code)];
	if (!was_down)
	{
		mKeyCodeDown[SCast<uint64>(key_code)] = true;
		EventData event_data;
		event_data.mKeyDown.mKeyCode = key_code;

		LoopOverEventFunctions(EventType::KeyDown, event_data);
	}
}

void EventManager::OnKeyUp(const SDL_Event& inEvent)
{
	KeyCode key_code = mSDLKeycodeToKeyCode[inEvent.key.keysym.sym];

	bool was_down = mKeyCodeDown[SCast<uint16>(key_code)];
	if (was_down)
	{
		mKeyCodeDown[SCast<uint64>(key_code)] = false;
		EventData event_data;
		event_data.mKeyDown.mKeyCode = key_code;

		LoopOverEventFunctions(EventType::KeyUp, event_data);
	}
}

void EventManager::OnMouseDown(const SDL_Event& inEvent)
{
	MouseButton mouse_button = mSDLButtonToMouseButton[inEvent.button.button];

	bool was_down = mMouseButtonDown[SCast<uint16>(mouse_button)];
	if (!was_down)
	{
		mMouseButtonDown[SCast<uint16>(mouse_button)] = true;
		EventData event_data;
		event_data.mMouseDown.mMouseButton = mouse_button;
		event_data.mMouseDown.mMousePosition = fm::vec2(SCast<float>(inEvent.button.x), SCast<float>(inEvent.button.y));

		LoopOverEventFunctions(EventType::MouseButtonDown, event_data);
	}
}

void EventManager::OnMouseUp(const SDL_Event& inEvent)
{
	MouseButton mouse_button = mSDLButtonToMouseButton[inEvent.button.button];

	bool was_down = mMouseButtonDown[SCast<uint16>(mouse_button)];
	if (was_down)
	{
		mMouseButtonDown[SCast<uint16>(mouse_button)] = false;
		EventData event_data;
		event_data.mMouseDown.mMouseButton = mouse_button;
		event_data.mMouseDown.mMousePosition = fm::vec2(SCast<float>(inEvent.button.x), SCast<float>(inEvent.button.y));

		LoopOverEventFunctions(EventType::MouseButtonUp, event_data);
	}
}

void EventManager::OnMouseMove(const SDL_Event& inEvent)
{
	mOldMousePosition = mMousePosition;
	mMousePosition = fm::vec2(SCast<float>(inEvent.motion.x), SCast<float>(inEvent.motion.y));

	EventData event_data;
	event_data.mMouseMove.mOldPosition = mOldMousePosition;
	event_data.mMouseMove.mNewPosition = mMousePosition;
	event_data.mMouseMove.mDelta = event_data.mMouseMove.mNewPosition - event_data.mMouseMove.mOldPosition;

	LoopOverEventFunctions(EventType::MouseMove, event_data);
}

void EventManager::LoopOverEventFunctions(EventType inEventType, const EventData& inEventData)
{
	Array<Array<WeakPtr<EventFunction>>::iterator> iterators_to_remove;
	Array<WeakPtr<EventFunction>> event_functions = mEventFunctions[inEventType];
	for (auto it = event_functions.begin(); it != event_functions.end(); ++it)
	{
		WeakPtr<EventFunction>& event_function = it.operator*();
		SharedPtr shared_function = event_function.lock();
		if (shared_function == nullptr)
		{
			iterators_to_remove.emplace_back(it);
			continue;
		}
		shared_function->mFunctionPtr(inEventData);
	}
	for (auto it = iterators_to_remove.rbegin(); it != iterators_to_remove.rend(); ++it)
	{
		event_functions.erase(*it);
	}
}
