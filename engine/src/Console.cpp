#include "Cute/Console.h"
#include "Cute/String.h"
#include "Cute/Exception.h"

CUTE_NS_BEGIN

#if defined(_WIN32)
// FOREGROUND_INTENSITY:前景高亮显示,BACKGROUND_INTENSITY 背景高亮显示
static WORD COLOR_FOREGROUND[Console::COLOR_MAX] =
{
	// BLACK
	0,                                            
	// RED
	FOREGROUND_RED,
	// GREEN
	FOREGROUND_GREEN,
	// BROWN
	FOREGROUND_RED | FOREGROUND_GREEN,
	// BLUE
	FOREGROUND_BLUE,
	// MAGENTA
	FOREGROUND_RED | FOREGROUND_BLUE,
	// CYAN
	FOREGROUND_GREEN | FOREGROUND_BLUE,
	// GREY
	FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, 
	// DARKGRAY
	FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	// RED_BOLD
	FOREGROUND_RED | FOREGROUND_INTENSITY,
	// GREEN_BOLD
	FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	// YELLOW
	FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	// BLUE_BOLD
	FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	// MAGENTA_BOLD
	FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	// CYAN_BOLD
	FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	// WHITE_BOLD
	FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
};
#else
enum AnsiTextAttr
{
	TA_NORMAL = 0,
	TA_BOLD = 1,
	TA_BLINK = 5,
	TA_REVERSE = 7
};
// 前景色
enum AnsiFGTextAttr
{
	FG_BLACK = 30, FG_RED, FG_GREEN, FG_BROWN, FG_BLUE,
	FG_MAGENTA, FG_CYAN, FG_WHITE, FG_YELLOW
};
// 背景色
enum AnsiBGTextAttr
{
	BG_BLACK = 40, BG_RED, BG_GREEN, BG_BROWN, BG_BLUE,
	BG_MAGENTA, BG_CYAN, BG_WHITE, BG_YELLOW
};

// 前景色对应数组
static int COLOR_FOREGROUND[Console::COLOR_MAX] =
{
	0x0000,
	0x0004,
	0x0002,
	0x0006,
	0x0001,
	0x0005,
	0x0003,
	0x0007,
	0x0008,
	0x000C,
	0x000A,
	0x000E,
	0x0009,
	0x000D,
	0x000B,
	0x000F
};
#endif

void Console::setColor(Color color /* = DEFAULT */)
{
#ifdef _WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, COLOR_FOREGROUND[color]);
#else
	fprintf(stdout, "\x1b[%d%sm", COLOR_FOREGROUND[color], (color > CC_YELLOW ? ";1" : ""));
#endif
}

void Console::resetColor()
{
	//
	setColor(GRAY);
}

Console::Color Console::parseColor(const String& color)
{
	if (icompare(color, "default") == 0)
		return DEFAULT;
	else if (icompare(color, "black") == 0)
		return BLACK;
	else if (icompare(color, "red") == 0)
		return RED;
	else if (icompare(color, "green") == 0)
		return GREEN;
	else if (icompare(color, "brown") == 0)
		return BROWN;
	else if (icompare(color, "blue") == 0)
		return BLUE;
	else if (icompare(color, "magenta") == 0)
		return MAGENTA;
	else if (icompare(color, "cyan") == 0)
		return CYAN;
	else if (icompare(color, "gray") == 0)
		return GRAY;
	else if (icompare(color, "darkGray") == 0)
		return DARKGRAY;
	else if (icompare(color, "lightRed") == 0)
		return LIGHTRED;
	else if (icompare(color, "lightGreen") == 0)
		return LIGHTGREEN;
	else if (icompare(color, "yellow") == 0)
		return YELLOW;
	else if (icompare(color, "lightBlue") == 0)
		return LIGHTBLUE;
	else if (icompare(color, "lightMagenta") == 0)
		return LIGHTMAGENTA;
	else if (icompare(color, "lightCyan") == 0)
		return LIGHTCYAN;
	else if (icompare(color, "white") == 0)
		return WHITE;
	else throw InvalidArgumentException("Invalid color value", color);
}

String Console::formatColor(Color color)
{
	switch (color)
	{
	case BLACK:        return "black";
	case RED:          return "red";
	case GREEN:        return "green";
	case BROWN:        return "brown";
	case BLUE:         return "blue";
	case MAGENTA:      return "magenta";
	case CYAN:         return "cyan";
	case GRAY:         return "gray";
	case DARKGRAY:     return "darkGray";
	case LIGHTRED:     return "lightRed";
	case LIGHTGREEN:   return "lightGreen";
	case YELLOW:       return "yellow";
	case LIGHTBLUE:    return "lightBlue";
	case LIGHTMAGENTA: return "lightMagenta";
	case LIGHTCYAN:    return "lightCyan";
	case WHITE:        return "white";
	default:           return "invalid";
	}
}

CUTE_NS_END