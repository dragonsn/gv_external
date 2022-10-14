// Implementation of base platform-agnostic platform functionality.
#include "Base.h"
#include "Platform.h"
#include "Game.h"
#if GP_WITH_LUA
	#include "ScriptController.h"
#endif
#include "Form.h"

namespace gameplay
{

void Platform::touchEventInternal(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex, bool actuallyMouse)
{
    if (actuallyMouse || !Form::touchEventInternal(evt, x, y, contactIndex))
    {
        Game::getInstance()->touchEvent(evt, x, y, contactIndex);
#if GP_WITH_LUA
        Game::getInstance()->getScriptController()->touchEvent(evt, x, y, contactIndex);
#endif
    }
}

void Platform::keyEventInternal(Keyboard::KeyEvent evt, int key)
{
    if (!Form::keyEventInternal(evt, key))
    {
        Game::getInstance()->keyEvent(evt, key);
#if GP_WITH_LUA
        Game::getInstance()->getScriptController()->keyEvent(evt, key);
#endif
    }
}

bool Platform::mouseEventInternal(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    if (Form::mouseEventInternal(evt, x, y, wheelDelta))
    {
        return true;
    }
    else if (Game::getInstance()->mouseEvent(evt, x, y, wheelDelta))
    {
        return true;
    }
    else
    {
#if GP_WITH_LUA
        return Game::getInstance()->getScriptController()->mouseEvent(evt, x, y, wheelDelta);
#else 
		return false;
#endif
    }

}

void Platform::gestureSwipeEventInternal(int x, int y, int direction)
{
    // TODO: Add support to Form for gestures
    Game::getInstance()->gestureSwipeEvent(x, y, direction);
#if GP_WITH_LUA
    Game::getInstance()->getScriptController()->gestureSwipeEvent(x, y, direction);
#endif
}

void Platform::gesturePinchEventInternal(int x, int y, float scale)
{
    // TODO: Add support to Form for gestures
    Game::getInstance()->gesturePinchEvent(x, y, scale);
#if GP_WITH_LUA
    Game::getInstance()->getScriptController()->gesturePinchEvent(x, y, scale);
#endif
}

void Platform::gestureTapEventInternal(int x, int y)
{
    // TODO: Add support to Form for gestures
    Game::getInstance()->gestureTapEvent(x, y);
#if GP_WITH_LUA
    Game::getInstance()->getScriptController()->gestureTapEvent(x, y);
#endif
}

void Platform::resizeEventInternal(unsigned int width, unsigned int height)
{
    // Update the width and height of the game
    Game* game = Game::getInstance();
    if (game->_width != width || game->_height != height)
    {
		if (!gv::gv_global::framework_config.is_editor)
		{
			game->_width = width;
			game->_height = height;
		}
        game->resizeEvent(width, height);
#if GP_WITH_LUA
        game->getScriptController()->resizeEvent(width, height);
#endif
    }
}

void Platform::gamepadEventInternal(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int analogIndex)
{
	switch(evt)
	{
	case Gamepad::CONNECTED_EVENT:
	case Gamepad::DISCONNECTED_EVENT:
		Game::getInstance()->gamepadEvent(evt, gamepad);
#if GP_WITH_LUA
        Game::getInstance()->getScriptController()->gamepadEvent(evt, gamepad);
#endif
		break;
	case Gamepad::BUTTON_EVENT:
	case Gamepad::JOYSTICK_EVENT:
	case Gamepad::TRIGGER_EVENT:
		Form::gamepadEventInternal(evt, gamepad, analogIndex);
		break;
	}
}

void Platform::gamepadEventConnectedInternal(GamepadHandle handle,  unsigned int buttonCount, unsigned int joystickCount, unsigned int triggerCount,
                                             unsigned int vendorId, unsigned int productId, const char* vendorString, const char* productString)
{
    Gamepad::add(handle, buttonCount, joystickCount, triggerCount, vendorId, productId, vendorString, productString);
}

void Platform::gamepadEventDisconnectedInternal(GamepadHandle handle)
{
    Gamepad::remove(handle);
}

}
