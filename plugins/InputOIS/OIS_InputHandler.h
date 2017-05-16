#pragma once
#include "OIS_API.h"
#include "Cute/InputHandler.h"

CUTE_NS_BEGIN

class CUTE_OIS_API OISInputHandler 
	: public InputHandler
	, public OIS::MouseListener
	, public OIS::KeyListener
	, public OIS::JoyStickListener
{
public:
	OISInputHandler();
	~OISInputHandler();

	void update();

private:
	bool keyPressed(const OIS::KeyEvent& arg) OVERRIDE;
	bool keyReleased(const OIS::KeyEvent &arg) OVERRIDE;

	bool mouseMoved(const OIS::MouseEvent &arg) OVERRIDE;
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) OVERRIDE;
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) OVERRIDE;

	bool buttonPressed(const OIS::JoyStickEvent &arg, int button) OVERRIDE;
	bool buttonReleased(const OIS::JoyStickEvent &arg, int button) OVERRIDE;
	bool axisMoved(const OIS::JoyStickEvent &arg, int axis) OVERRIDE;

private:
	ButtonCode getKeyCode(OIS::KeyCode code);
	ButtonCode getMouseCode(OIS::MouseButtonID code);
	ButtonCode getJoyStickCode(INT32 code);

private:
	typedef std::vector<OIS::JoyStick*> OISJoyStickList;
	OIS::InputManager*	m_manager;
	OIS::Mouse*			m_mouse;
	OIS::Keyboard*		m_keybord;
	OISJoyStickList		m_joysticks;
};

CUTE_NS_END
