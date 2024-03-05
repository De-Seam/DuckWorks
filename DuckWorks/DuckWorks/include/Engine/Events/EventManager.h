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

class EventManager
{
public:
	EventManager() = default;
	~EventManager() = default;

	void Init();
	void SetupSDLConversions();

	void Update(float inDeltaTime);

	using EventFunction = std::function<void(EventData)>;
	void AddEventFunction(EventType inEventType, const EventFunction& inFunctionPtr);

private:
	HashMap<EventType, Array<EventFunction>> mEventFunctions;
	BitBool<SCast<uint8>(KeyCode::COUNT)> mKeyCodeDown = { false };
	BitBool<SCast<uint8>(MouseButton::COUNT)> mMouseButtonDown = { false };

	HashMap<SDL_Keycode, KeyCode> mSDLKeycodeToKeyCode;
	HashMap<uint8, MouseButton> mSDLButtonToMouseButton;

private:
	void OnKeyDown(const SDL_Event& inEvent);
	void OnKeyUp(const SDL_Event& inEvent);

	void OnMouseDown(const SDL_Event& inEvent);
	void OnMouseUp(const SDL_Event& inEvent);
};

extern EventManager gEventManager;