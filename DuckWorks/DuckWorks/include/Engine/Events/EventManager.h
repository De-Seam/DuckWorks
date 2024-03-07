#pragma once
#include "Core/CoreBase.h"

// External includes
#include <External/SDL/SDL_keycode.h>
#include <External/SDL/SDL_events.h>
#include <External/BitBool/BitBool.h>

// Std includes
#include <functional>

enum class KeyCode : uint8
{
	A = 0,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	Zero,
	One,
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	Delete,

	COUNT
};

enum class MouseButton : uint8
{
	Left = 0,
	Right,
	Middle,
	X1,
	X2,

	COUNT
};

enum class EventType : uint8
{
	Invalid = 0,
	KeyDown,
	KeyUp,
	MouseMove,
	MouseButtonDown,
	MouseButtonUp,
	MouseWheel,

	COUNT
};

class EventManager
{
public:
	EventManager() = default;
	~EventManager() = default;

	void Init();
	void SetupSDLConversions();

	void Update(float inDeltaTime);

	struct EventData
	{
		EventData()
		{
			memset(this, 0, sizeof(EventData));
		}

		EventData(const EventData& inEventData)
		{
			memcpy(this, &inEventData, sizeof(EventData));
		}

		union
		{
			struct KeyDown // KeyDown
			{
				KeyCode mKeyCode;
			} mKeyDown;

			struct KeyUp // KeyUp
			{
				KeyCode mKeyCode;
			} mKeyUp;

			struct MouseMove // MouseMove
			{
				fm::vec2 mOldPosition;
				fm::vec2 mNewPosition;
				float mDelta;
			} mMouseMove;

			struct MouseDown // MouseDown
			{
				MouseButton mMouseButton;
			} mMouseDown;

			struct MouseUp // MouseUp
			{
				MouseButton mMouseButton;
			} mMouseUp;

			struct MouseWheel //MouseWheel
			{
				float mDelta;
			} mMouseWheel;
		};
	};

	using EventFunctionPtr = std::function<void(const EventData&)>;

	struct EventFunction
	{
		EventFunction() = default;

		EventFunction(EventType inEventType, const EventFunctionPtr& inFunctionPtr)
			: mEventType(inEventType), mFunctionPtr(inFunctionPtr) {}

		EventType mEventType = EventType::Invalid;
		EventFunctionPtr mFunctionPtr = nullptr;
	};

	SharedPtr<EventFunction> AddEventFunction(const EventFunction& inFunction);
	void AddPersistentEventFunction(const EventFunction& inFunction);

private:
	HashMap<EventType, Array<WeakPtr<EventFunction>>> mEventFunctions = {};
	Array<SharedPtr<EventFunction>> mPersistentEventFunctions;

	BitBool<SCast<uint16>(KeyCode::COUNT)> mKeyCodeDown = {false};
	BitBool<SCast<uint16>(MouseButton::COUNT)> mMouseButtonDown = {false};

	HashMap<SDL_Keycode, KeyCode> mSDLKeycodeToKeyCode;
	HashMap<uint32, MouseButton> mSDLButtonToMouseButton;

private:
	void OnKeyDown(const SDL_Event& inEvent);
	void OnKeyUp(const SDL_Event& inEvent);

	void OnMouseDown(const SDL_Event& inEvent);
	void OnMouseUp(const SDL_Event& inEvent);

	void LoopOverEventFunctions(EventType inEventType, const EventData& inEventData);
};

extern EventManager gEventManager;
