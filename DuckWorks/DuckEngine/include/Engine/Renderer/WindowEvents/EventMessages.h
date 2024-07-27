#pragma once

#include <External/SFML/Window/Event.hpp>

struct MsgWindowClosed : public MsgBase
{
	RTTI_MSG()
};

struct MsgWindowResized : public MsgBase
{
	RTTI_MSG()

	uint32 mWidth = 0, mHeight = 0;
};

struct MsgWindowLostFocus : public MsgBase
{
	RTTI_MSG()
};

struct MsgWindowGainedFocus : public MsgBase
{
	RTTI_MSG()
};

struct MsgWindowTextEntered : public MsgBase
{
	RTTI_MSG()

	uint32 mUnicodeCharacter = 0;
};

struct MsgWindowKeyPressed : public MsgBase
{
	RTTI_MSG()

	sf::Keyboard::Key mKey = sf::Keyboard::Unknown;
	bool mAltDown = false;
	bool mControlDown = false;
	bool mShiftDown = false;
	bool mSystemKeyDown = false;
};

struct MsgWindowKeyReleased : public MsgBase
{
	RTTI_MSG()

	sf::Keyboard::Key mKey = sf::Keyboard::Unknown;
	bool mAltDown = false;
	bool mControlDown = false;
	bool mShiftDown = false;
	bool mSystemKeyDown = false;
};      

struct MsgWindowMouseWheelMoved : public MsgBase
{
	RTTI_MSG()

	int32 mDelta = 0; ///< Number of ticks the wheel has moved (positive is up, negative is down)
    int32 mX = 0; ///< X position of the mouse pointer, relative to the left of the owner window
    int32 mY = 0; ///< Y position of the mouse pointer, relative to the top of the owner window
};

struct MsgWindowMouseWheelScrolled : public MsgBase
{
	RTTI_MSG()

	sf::Mouse::Wheel mWheel = sf::Mouse::VerticalWheel; ///< Which wheel (for mice with multiple ones)
	float mDelta = 0; ///< Number of ticks the wheel has moved (positive is up, negative is down)
    int32 mX = 0; ///< X position of the mouse pointer, relative to the left of the owner window
    int32 mY = 0; ///< Y position of the mouse pointer, relative to the top of the owner window
};

struct MsgWindowMouseButtonPressed : public MsgBase
{
	RTTI_MSG()

	sf::Event::MouseButtonEvent mButtonEvent;
};

struct MsgWindowMouseButtonReleased : public MsgBase
{
	RTTI_MSG()

	sf::Event::MouseButtonEvent mButtonEvent;
};

struct MsgWindowMouseMoved : public MsgBase
{
	RTTI_MSG()

	sf::Event::MouseMoveEvent mMouseMoveEvent;
};

struct MsgWindowMouseEntered : public MsgBase
{
	RTTI_MSG()
};

struct MsgWindowMouseLeft : public MsgBase
{
	RTTI_MSG()
};

struct MsgWindowJoystickButtonPressed : public MsgBase
{
	RTTI_MSG()

	sf::Event::JoystickButtonEvent mJoystickButtonEvent;
};

struct MsgWindowJoystickButtonReleased : public MsgBase
{
	RTTI_MSG()

	sf::Event::JoystickButtonEvent mJoystickButtonEvent;
};

struct MsgWindowJoystickMoved : public MsgBase
{
	RTTI_MSG()

	sf::Event::JoystickMoveEvent mJoystickMoveEvent;
};

struct MsgWindowJoystickConnected : public MsgBase
{
	RTTI_MSG()

	sf::Event::JoystickConnectEvent mJoystickConnectEvent;
};

struct MsgWindowJoystickDisconnected : public MsgBase
{
	RTTI_MSG()

	sf::Event::JoystickConnectEvent mJoystickConnectEvent;
};

struct MsgWindowTouchBegan : public MsgBase
{
	RTTI_MSG()

	sf::Event::TouchEvent mTouchEvent;
};

struct MsgWindowTouchMoved : public MsgBase
{
	RTTI_MSG()

	sf::Event::TouchEvent mTouchEvent;
};

struct MsgWindowTouchEnded : public MsgBase
{
	RTTI_MSG()

	sf::Event::TouchEvent mTouchEvent;
};

struct MsgWindowSensorChanged : public MsgBase
{
	RTTI_MSG()

	sf::Event::SensorEvent mSensorEvent;
};