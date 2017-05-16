#include "OIS_InputHandler.h"

CUTE_NS_BEGIN

OISInputHandler::OISInputHandler()
	: m_manager(NULL)
	, m_mouse(NULL)
	, m_keybord(NULL)
{
	OIS::ParamList pl;
	
#if defined CUTE_OS_FAMILY_WINDOWS
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined CUTE_OS_FAMILY_POSIX
	pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
	pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
	pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
	pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif

	m_manager = OIS::InputManager::createInputSystem(pl);

	if (m_manager->getNumberOfDevices(OIS::OISKeyboard) > 0)
	{
		m_keybord = (OIS::Keyboard*)m_manager->createInputObject(OIS::OISKeyboard, true);
		m_keybord->setEventCallback(this);
	}

	if (m_manager->getNumberOfDevices(OIS::OISMouse) > 0)
	{
		m_mouse = (OIS::Mouse*)m_manager->createInputObject(OIS::OISMouse, true);
		m_mouse->setEventCallback(this);
	}

	int joystickNum = m_manager->getNumberOfDevices(OIS::OISJoyStick);
	for (int i = 0; i < joystickNum; ++i)
	{
		OIS::JoyStick* joystick = (OIS::JoyStick*)m_manager->createInputObject(OIS::OISJoyStick, true);
		joystick->setEventCallback(this);
		m_joysticks.push_back(joystick);
	}

	//
}

OISInputHandler::~OISInputHandler()
{
	// ÊÍ·Å
	if (m_manager != NULL)
	{
		if (m_mouse != NULL)
			m_manager->destroyInputObject(m_mouse);

		if (m_keybord != NULL)
			m_manager->destroyInputObject(m_keybord);

		for (size_t i = 0; i < m_joysticks.size(); ++i)
			m_manager->destroyInputObject(m_joysticks[i]);

		m_joysticks.clear();
	}
}

void OISInputHandler::update()
{
	if (m_mouse != NULL)
		m_mouse->capture();

	if (m_keybord)
		m_keybord->capture();

	for (OISJoyStickList::iterator itor = m_joysticks.begin(); itor != m_joysticks.end(); ++itor)
	{
		(*itor)->capture();
	}
}

bool OISInputHandler::keyPressed(const OIS::KeyEvent& arg)
{
	onButtonDown(getKeyCode(arg.key), 0, arg.timestamp);
	return true;
}

bool OISInputHandler::keyReleased(const OIS::KeyEvent& arg)
{
	onButtonUp(getKeyCode(arg.key), 0, arg.timestamp);
	return true;
}

bool OISInputHandler::mouseMoved(const OIS::MouseEvent &arg)
{
	return true;
}

bool OISInputHandler::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	onButtonDown(getMouseCode(id), 0, arg.timestamp);
	return true;
}

bool OISInputHandler::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	onButtonUp(getMouseCode(id), 0, arg.timestamp);
	return true;
}

bool OISInputHandler::buttonPressed(const OIS::JoyStickEvent &arg, int button)
{
	onButtonDown(getJoyStickCode(button), arg.device->getID(), 0);
	return true;
}

bool OISInputHandler::buttonReleased(const OIS::JoyStickEvent &arg, int button)
{
	onButtonUp(getJoyStickCode(button), arg.device->getID(), 0);
	return true;
}

bool OISInputHandler::axisMoved(const OIS::JoyStickEvent &arg, int axis)
{
	return true;
}

ButtonCode OISInputHandler::getKeyCode(OIS::KeyCode code)
{
	return (ButtonCode)code;
}

ButtonCode OISInputHandler::getMouseCode(OIS::MouseButtonID code)
{
	return (ButtonCode)(code + BC_MOUSE_LEFT);
}

ButtonCode OISInputHandler::getJoyStickCode(INT32 code)
{
	// TODO:
	switch (code)
	{
	case 0:
		return BC_GAMEPAD_DPAD_UP;
	case 1:
		return BC_GAMEPAD_DPAD_DOWN;
	case 2:
		return BC_GAMEPAD_DPAD_LEFT;
	case 3:
		return BC_GAMEPAD_DPAD_RIGHT;
	case 4:
		return BC_GAMEPAD_START;
	case 5:
		return BC_GAMEPAD_BACK;
	case 6:
		return BC_GAMEPAD_LS;
	case 7:
		return BC_GAMEPAD_RS;
	case 8:
		return BC_GAMEPAD_LB;
	case 9:
		return BC_GAMEPAD_RB;
	case 10:
		return BC_GAMEPAD_BTN1;
	case 11:
		return BC_GAMEPAD_LS;
	case 12:
		return BC_GAMEPAD_A;
	case 13:
		return BC_GAMEPAD_B;
	case 14:
		return BC_GAMEPAD_X;
	case 15:
		return BC_GAMEPAD_Y;
	}
	return (ButtonCode)(BC_GAMEPAD_A + code - 15);
}

CUTE_NS_END
