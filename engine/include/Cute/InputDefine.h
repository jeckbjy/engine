//! Input
#pragma once
#include "Cute/Foundation.h"
#include "Cute/Vector2.h"
#include "Cute/Vector3.h"
#include "Cute/Quaternion.h"

CUTE_NS_BEGIN

//enum KeyCode
//{
//
//};
//
//enum MouseCode
//{
//
//};
//
//enum GamepadCode
//{
//
//};

enum ButtonCode
{
	BC_UNASSIGNED		= 0x00,
	BC_KEY_ESCAPE		= 0x01,
	BC_KEY_1			= 0x02,
	BC_KEY_2			= 0x03,
	BC_KEY_3			= 0x04,
	BC_KEY_4			= 0x05,
	BC_KEY_5			= 0x06,
	BC_KEY_6			= 0x07,
	BC_KEY_7			= 0x08,
	BC_KEY_8			= 0x09,
	BC_KEY_9			= 0x0A,
	BC_KEY_0			= 0x0B,
	BC_KEY_MINUS		= 0x0C,    // - on main keyboard
	BC_KEY_EQUALS		= 0x0D,
	BC_KEY_BACK			= 0x0E,    // backspace
	BC_KEY_TAB			= 0x0F,
	BC_KEY_Q			= 0x10,
	BC_KEY_W			= 0x11,
	BC_KEY_E			= 0x12,
	BC_KEY_R			= 0x13,
	BC_KEY_T			= 0x14,
	BC_KEY_Y			= 0x15,
	BC_KEY_U			= 0x16,
	BC_KEY_I			= 0x17,
	BC_KEY_O			= 0x18,
	BC_KEY_P			= 0x19,
	BC_KEY_LBRACKET		= 0x1A,
	BC_KEY_RBRACKET		= 0x1B,
	BC_KEY_RETURN		= 0x1C,    // Enter on main keyboard
	BC_KEY_LCONTROL		= 0x1D,
	BC_KEY_A			= 0x1E,
	BC_KEY_S			= 0x1F,
	BC_KEY_D			= 0x20,
	BC_KEY_F			= 0x21,
	BC_KEY_G			= 0x22,
	BC_KEY_H			= 0x23,
	BC_KEY_J			= 0x24,
	BC_KEY_K			= 0x25,
	BC_KEY_L			= 0x26,
	BC_KEY_SEMICOLON	= 0x27,
	BC_KEY_APOSTROPHE	= 0x28,
	BC_KEY_GRAVE		= 0x29,    // accent
	BC_KEY_LSHIFT		= 0x2A,
	BC_KEY_BACKSLASH	= 0x2B,
	BC_KEY_Z			= 0x2C,
	BC_KEY_X			= 0x2D,
	BC_KEY_C			= 0x2E,
	BC_KEY_V			= 0x2F,
	BC_KEY_B			= 0x30,
	BC_KEY_N			= 0x31,
	BC_KEY_M			= 0x32,
	BC_KEY_COMMA		= 0x33,
	BC_KEY_PERIOD		= 0x34,    // . on main keyboard
	BC_KEY_SLASH		= 0x35,    // / on main keyboard
	BC_KEY_RSHIFT		= 0x36,
	BC_KEY_MULTIPLY		= 0x37,    // * on numeric keypad
	BC_KEY_LMENU		= 0x38,    // left Alt
	BC_KEY_SPACE		= 0x39,
	BC_KEY_CAPITAL		= 0x3A,
	BC_KEY_F1			= 0x3B,
	BC_KEY_F2			= 0x3C,
	BC_KEY_F3			= 0x3D,
	BC_KEY_F4			= 0x3E,
	BC_KEY_F5			= 0x3F,
	BC_KEY_F6			= 0x40,
	BC_KEY_F7			= 0x41,
	BC_KEY_F8			= 0x42,
	BC_KEY_F9			= 0x43,
	BC_KEY_F10			= 0x44,
	BC_KEY_NUMLOCK		= 0x45,
	BC_KEY_SCROLL		= 0x46,    // Scroll Lock
	BC_KEY_NUMPAD7		= 0x47,
	BC_KEY_NUMPAD8		= 0x48,
	BC_KEY_NUMPAD9		= 0x49,
	BC_KEY_SUBTRACT		= 0x4A,    // - on numeric keypad
	BC_KEY_NUMPAD4		= 0x4B,
	BC_KEY_NUMPAD5		= 0x4C,
	BC_KEY_NUMPAD6		= 0x4D,
	BC_KEY_ADD			= 0x4E,    // + on numeric keypad
	BC_KEY_NUMPAD1		= 0x4F,
	BC_KEY_NUMPAD2		= 0x50,
	BC_KEY_NUMPAD3		= 0x51,
	BC_KEY_NUMPAD0		= 0x52,
	BC_KEY_DECIMAL		= 0x53,    // . on numeric keypad
	BC_KEY_OEM_102		= 0x56,    // < > | on UK/Germany keyboards
	BC_KEY_F11			= 0x57,
	BC_KEY_F12			= 0x58,
	BC_KEY_F13			= 0x64,    //                     (NEC PC98)
	BC_KEY_F14			= 0x65,    //                     (NEC PC98)
	BC_KEY_F15			= 0x66,    //                     (NEC PC98)
	BC_KEY_KANA			= 0x70,    // (Japanese keyboard)
	BC_KEY_ABNT_C1		= 0x73,    // / ? on Portugese (Brazilian) keyboards
	BC_KEY_CONVERT		= 0x79,    // (Japanese keyboard)
	BC_KEY_NOCONVERT	= 0x7B,    // (Japanese keyboard)
	BC_KEY_YEN			= 0x7D,    // (Japanese keyboard)
	BC_KEY_ABNT_C2		= 0x7E,    // Numpad . on Portugese (Brazilian) keyboards
	BC_KEY_NUMPADEQUALS = 0x8D,    // = on numeric keypad (NEC PC98)
	BC_KEY_PREVTRACK	= 0x90,    // Previous Track (BC_CIRCUMFLEX on Japanese keyboard)
	BC_KEY_AT			= 0x91,    //                     (NEC PC98)
	BC_KEY_COLON		= 0x92,    //                     (NEC PC98)
	BC_KEY_UNDERLINE	= 0x93,    //                     (NEC PC98)
	BC_KEY_KANJI		= 0x94,    // (Japanese keyboard)
	BC_KEY_STOP			= 0x95,    //                     (NEC PC98)
	BC_KEY_AX			= 0x96,    //                     (Japan AX)
	BC_KEY_UNLABELED	= 0x97,    //                     (J3100)
	BC_KEY_NEXTTRACK	= 0x99,    // Next Track
	BC_KEY_NUMPADENTER	= 0x9C,    // Enter on numeric keypad
	BC_KEY_RCONTROL		= 0x9D,
	BC_KEY_MUTE			= 0xA0,    // Mute
	BC_KEY_CALCULATOR	= 0xA1,    // Calculator
	BC_KEY_PLAYPAUSE	= 0xA2,    // Play / Pause
	BC_KEY_MEDIASTOP	= 0xA4,    // Media Stop
	BC_KEY_VOLUMEDOWN	= 0xAE,    // Volume -
	BC_KEY_VOLUMEUP		= 0xB0,    // Volume +
	BC_KEY_WEBHOME		= 0xB2,    // Web home
	BC_KEY_NUMPADCOMMA	= 0xB3,    // , on numeric keypad (NEC PC98)
	BC_KEY_DIVIDE		= 0xB5,    // / on numeric keypad
	BC_KEY_SYSRQ		= 0xB7,
	BC_KEY_RMENU		= 0xB8,    // right Alt
	BC_KEY_PAUSE		= 0xC5,    // Pause
	BC_KEY_HOME			= 0xC7,    // Home on arrow keypad
	BC_KEY_UP			= 0xC8,    // UpArrow on arrow keypad
	BC_KEY_PGUP			= 0xC9,    // PgUp on arrow keypad
	BC_KEY_LEFT			= 0xCB,    // LeftArrow on arrow keypad
	BC_KEY_RIGHT		= 0xCD,    // RightArrow on arrow keypad
	BC_KEY_END			= 0xCF,    // End on arrow keypad
	BC_KEY_DOWN			= 0xD0,    // DownArrow on arrow keypad
	BC_KEY_PGDOWN		= 0xD1,    // PgDn on arrow keypad
	BC_KEY_INSERT		= 0xD2,    // Insert on arrow keypad
	BC_KEY_DELETE		= 0xD3,    // Delete on arrow keypad
	BC_KEY_LWIN			= 0xDB,    // Left Windows key
	BC_KEY_RWIN			= 0xDC,    // Right Windows key
	BC_KEY_APPS			= 0xDD,    // AppMenu key
	BC_KEY_POWER		= 0xDE,    // System Power
	BC_KEY_SLEEP		= 0xDF,    // System Sleep
	BC_KEY_WAKE			= 0xE3,    // System Wake
	BC_KEY_WEBSEARCH	= 0xE5,    // Web Search
	BC_KEY_WEBFAVORITES = 0xE6,    // Web Favorites
	BC_KEY_WEBREFRESH	= 0xE7,    // Web Refresh
	BC_KEY_WEBSTOP		= 0xE8,    // Web Stop
	BC_KEY_WEBFORWARD	= 0xE9,    // Web Forward
	BC_KEY_WEBBACK		= 0xEA,    // Web Back
	BC_KEY_MYCOMPUTER	= 0xEB,    // My Computer
	BC_KEY_MAIL			= 0xEC,    // Mail
	BC_KEY_MEDIASELECT	= 0xED,    // Media Select
	BC_MOUSE_LEFT		= 0xEE,		// Mouse buttons - Most important bit signifies this key is a mouse button
	BC_MOUSE_RIGHT		= 0xEF,
	BC_MOUSE_MIDDLE		= 0xF0,
	BC_MOUSE_BTN4		= 0xF1,
	BC_MOUSE_BTN5		= 0xF2,
	BC_MOUSE_BTN6		= 0xF3,
	BC_MOUSE_BTN7		= 0xF4,
	BC_MOUSE_BTN8		= 0xF5,
	BC_MOUSE_BTN9		= 0xF6,
	BC_MOUSE_BTN10		= 0xF7,
	BC_MOUSE_BTN11		= 0xF8,
	BC_MOUSE_BTN12		= 0xF9,
	BC_MOUSE_BTN13		= 0xFA,
	BC_MOUSE_BTN14		= 0xFB,
	BC_MOUSE_BTN15		= 0xFC,
	BC_MOUSE_BTN16		= 0xFD,
	BC_MOUSE_BTN17		= 0xFE,
	BC_MOUSE_BTN18		= 0xFF,
	BC_MOUSE_BTN19		= 0x101,
	BC_MOUSE_BTN20		= 0x102,
	BC_MOUSE_BTN21		= 0x103,
	BC_MOUSE_BTN22		= 0x104,
	BC_MOUSE_BTN23		= 0x105,
	BC_MOUSE_BTN24		= 0x106,
	BC_MOUSE_BTN25		= 0x107,
	BC_MOUSE_BTN26		= 0x108,
	BC_MOUSE_BTN27		= 0x109,
	BC_MOUSE_BTN28		= 0x10A,
	BC_MOUSE_BTN29		= 0x10B,
	BC_MOUSE_BTN30		= 0x10C,
	BC_MOUSE_BTN31		= 0x10D,
	BC_MOUSE_BTN32		= 0x10E,
	BC_GAMEPAD_A		= 0x10F, // Joystick/Gamepad buttons- Second most important bit signifies key is a gamepad button
	BC_GAMEPAD_B		= 0x110, // Similar to keyboard names, these are for convenience named after Xbox controller buttons
	BC_GAMEPAD_X		= 0x111, // but if some other controller is connected you will need to learn yourself which of these
	BC_GAMEPAD_Y		= 0x112, // corresponds to which actual button on the controller.
	BC_GAMEPAD_LB		= 0x113,
	BC_GAMEPAD_RB		= 0x114,
	BC_GAMEPAD_LS		= 0x115,
	BC_GAMEPAD_RS		= 0x116,
	BC_GAMEPAD_BACK		= 0x117,
	BC_GAMEPAD_START	= 0x118,
	BC_GAMEPAD_DPAD_LEFT= 0x119,
	BC_GAMEPAD_DPAD_RIGHT= 0x11A,
	BC_GAMEPAD_DPAD_UP	= 0x11B,
	BC_GAMEPAD_DPAD_DOWN= 0x11C,
	BC_GAMEPAD_BTN1		= 0x11D,
	BC_GAMEPAD_BTN2		= 0x11E,
	BC_GAMEPAD_BTN3		= 0x11F,
	BC_GAMEPAD_BTN4		= 0x120,
	BC_GAMEPAD_BTN5		= 0x121,
	BC_GAMEPAD_BTN6		= 0x122,
	BC_GAMEPAD_BTN7		= 0x123,
	BC_GAMEPAD_BTN8		= 0x124,
	BC_GAMEPAD_BTN9		= 0x125,
	BC_GAMEPAD_BTN10	= 0x126,
	BC_GAMEPAD_BTN11	= 0x127,
	BC_GAMEPAD_BTN12	= 0x128,
	BC_GAMEPAD_BTN13	= 0x129,
	BC_GAMEPAD_BTN14	= 0x12A,
	BC_GAMEPAD_BTN15	= 0x12B,
	BC_GAMEPAD_BTN16	= 0x12C,
	BC_GAMEPAD_BTN17	= 0x12D,
	BC_GAMEPAD_BTN18	= 0x12E,
	BC_GAMEPAD_BTN19	= 0x12F,
	BC_GAMEPAD_BTN20	= 0x130,
	BC_COUNT			= 0x131,
};

enum class TouchPhase
{
	Begin,
	Moved,
	Stationary,
	Ended,
	Canceled,
};

// 手指触摸
struct CUTE_CORE_API Touch
{
	uint32		fingerId;
	Vector2		position;
	Vector2		deltaPosition;
	float		deltaTime;
	int			tapCount;
	TouchPhase	phase;
};

// 陀螺仪
struct CUTE_CORE_API Gyroscope
{
	Vector3		rotationRate;			//旋转速率
	Vector3		rotationRateUnbiased;	//无偏旋转速率
	Vector3		gravity;				//重力加速度
	Vector3		userAcceleration;		//用户加速度
	Quaternion	attitude;				//姿态
	bool		enabled;				//启用状态
	float		updateInterval;			//设置或检索该陀螺仪的间隔，以秒为单位;
};

CUTE_NS_END
