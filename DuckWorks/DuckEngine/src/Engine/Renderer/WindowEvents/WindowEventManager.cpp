#include <Engine/Precomp.h>
#include <Engine/Renderer/WindowEvents/WindowEventManager.h>

// Core includes
#include <Core/Utilities/Assert.h>
#include <Core/Utilities/Profiler.h>

// Engine includes
#include <Engine/Renderer/Renderer.h>

// External includes
#include <External/SFML/Graphics.hpp>

WindowEventManager::WindowEventManager()
{
}

void WindowEventManager::Update(Renderer& inRenderer, float)
{
	PROFILE_SCOPE(WindowEventManager::Update)

	sf::RenderWindow& render_window = inRenderer.GetRenderWindow();
	sf::Event event;
	while (render_window.pollEvent(event))
	{
		MsgAnyWindowEvent msgAnyWindowEvent;
		msgAnyWindowEvent.mEvent = event;
		BroadcastMessage(msgAnyWindowEvent);
		switch(event.type)
		{
			case sf::Event::Closed:					// The window requested to be closed (no data)
			{
				MsgWindowClosed msg;
				BroadcastMessage(msg);
				break;
			}
			case sf::Event::Resized:				// The window was resized (data in event.size)
			{
				MsgWindowResized msg;
				msg.mWidth = event.size.width;
				msg.mHeight = event.size.height;
				BroadcastMessage(msg);
				break;
			}
			case sf::Event::LostFocus:				// The window lost the focus (no data)
			{
				MsgWindowLostFocus msg;
				BroadcastMessage(msg);
				break;
			}
			case sf::Event::GainedFocus:			// The window gained the focus (no data)
			{
				MsgWindowGainedFocus msg;
				BroadcastMessage(msg);
				break;
			}
			case sf::Event::TextEntered:			// A character was entered (data in event.text)
			{
				MsgWindowTextEntered msg;
				msg.mUnicodeCharacter = event.text.unicode;
				BroadcastMessage(msg);
				break;
			}
			case sf::Event::KeyPressed:				// A key was pressed (data in event.key)
			{
				MsgWindowKeyPressed msg;
				msg.mKey = event.key.code;
				msg.mAltDown = event.key.alt;
				msg.mControlDown = event.key.control;
				msg.mShiftDown = event.key.shift;
				msg.mSystemKeyDown = event.key.system;
				BroadcastMessage(msg);
				break;
			}
			case sf::Event::KeyReleased:			// A key was released (data in event.key)
			{
				MsgWindowKeyReleased msg;
				msg.mKey = event.key.code;
				msg.mAltDown = event.key.alt;
				msg.mControlDown = event.key.control;
				msg.mShiftDown = event.key.shift;
				msg.mSystemKeyDown = event.key.system;
				BroadcastMessage(msg);
				break;
			}
			case sf::Event::MouseWheelMoved:		// The mouse wheel was scrolled (data in event.mouseWheel) (deprecated)
			{
				MsgWindowMouseWheelMoved msg;
				msg.mDelta = event.mouseWheel.delta;
				msg.mX = event.mouseWheel.x;
				msg.mY = event.mouseWheel.y;
				BroadcastMessage(msg);
				break;
			}
			case sf::Event::MouseWheelScrolled:		// The mouse wheel was scrolled (data in event.mouseWheelScroll)
			{
				MsgWindowMouseWheelScrolled msg;
				msg.mWheel = event.mouseWheelScroll.wheel;
				msg.mDelta = event.mouseWheelScroll.delta;
				msg.mX = event.mouseWheelScroll.x;
				msg.mY = event.mouseWheelScroll.y;
				BroadcastMessage(msg);
				break;
			}
			case sf::Event::MouseButtonPressed:		// A mouse button was pressed (data in event.mouseButton)
			{
				MsgWindowMouseButtonPressed msg;
				msg.mButtonEvent = event.mouseButton;
				BroadcastMessage(msg);
				break;
			}
			case sf::Event::MouseButtonReleased:	// A mouse button was released (data in event.mouseButton)
			{
				MsgWindowMouseButtonReleased msg;
				msg.mButtonEvent = event.mouseButton;
				BroadcastMessage(msg);
				break;
			}
			case sf::Event::MouseMoved:				// The mouse cursor moved (data in event.mouseMove)
			{
				MsgWindowMouseMoved msg;
				msg.mMouseMoveEvent = event.mouseMove;
				BroadcastMessage(msg);
				break;
			}
			case sf::Event::MouseEntered:			// The mouse cursor entered the area of the window (no data)
			{
				MsgWindowMouseEntered msg;
				BroadcastMessage(msg);
				break;
			}
			case sf::Event::MouseLeft:				// The mouse cursor left the area of the window (no data)
			{
				MsgWindowMouseLeft msg;
				BroadcastMessage(msg);
				break;
			}
			case sf::Event::JoystickButtonPressed:	// A joystick button was pressed (data in event.joystickButton)
			{
				MsgWindowJoystickButtonPressed msg;
				msg.mJoystickButtonEvent = event.joystickButton;
				BroadcastMessage(msg);
				break;
			}
			case sf::Event::JoystickButtonReleased:	// A joystick button was released (data in event.joystickButton)
			{
				MsgWindowJoystickButtonReleased msg;
				msg.mJoystickButtonEvent = event.joystickButton;
				BroadcastMessage(msg);
				break;
			}
			case sf::Event::JoystickMoved:			// The joystick moved along an axis (data in event.joystickMove)
			{
				MsgWindowJoystickMoved msg;
				msg.mJoystickMoveEvent = event.joystickMove;
				BroadcastMessage(msg);
				break;
			}
			case sf::Event::JoystickConnected:		// A joystick was connected (data in event.joystickConnect)
			{
				MsgWindowJoystickConnected msg;
				msg.mJoystickConnectEvent = event.joystickConnect;
				BroadcastMessage(msg);
				break;
			}
			case sf::Event::JoystickDisconnected:	// A joystick was disconnected (data in event.joystickConnect)
			{
				MsgWindowJoystickDisconnected msg;
				msg.mJoystickConnectEvent = event.joystickConnect;
				BroadcastMessage(msg);
				break;
			}
			case sf::Event::TouchBegan:				// A touch event began (data in event.touch)
			{
				MsgWindowTouchBegan msg;
				msg.mTouchEvent = event.touch;
				BroadcastMessage(msg);
				break;
			}
			case sf::Event::TouchMoved:				// A touch moved (data in event.touch)
			{
				MsgWindowTouchMoved msg;
				msg.mTouchEvent = event.touch;
				BroadcastMessage(msg);
				break;
			}
			case sf::Event::TouchEnded:				// A touch event ended (data in event.touch)
			{
				MsgWindowTouchEnded msg;
				msg.mTouchEvent = event.touch;
				BroadcastMessage(msg);
				break;
			}
			case sf::Event::SensorChanged:			// A sensor value changed (data in event.sensor)
			{
				MsgWindowSensorChanged msg;
				msg.mSensorEvent = event.sensor;
				BroadcastMessage(msg);
				break;
			}
			default:
				gAssert(false && "Unhandled window event");
				break;
		}
	}
}
