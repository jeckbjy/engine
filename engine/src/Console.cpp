#include "Console.h"

CU_NS_BEGIN

// 控制台颜色
#ifdef CU_OS_WIN
// 事件回调,忽略消息返回false
BOOL WINAPI ConsoleQuitHandler(DWORD cevent)
{
	switch (cevent)
	{
	case CTRL_C_EVENT:			// ctrl + c
	case CTRL_BREAK_EVENT:		// ctrl + break
	case CTRL_CLOSE_EVENT:		// 点击close按钮
		return false;
	case CTRL_LOGOFF_EVENT:		// 用户退出
	case CTRL_SHUTDOWN_EVENT:	// 系统关闭
		return true;
	}
	return true;
}

// FOREGROUND_INTENSITY:前景高亮显示,BACKGROUND_INTENSITY 背景高亮显示
static WORD WinColorFG[CC_COLOR_MAX] =
{
	0,                                                  // BLACK
	FOREGROUND_RED,                                     // RED
	FOREGROUND_GREEN,                                   // GREEN
	FOREGROUND_RED | FOREGROUND_GREEN,                  // BROWN
	FOREGROUND_BLUE,                                    // BLUE
	FOREGROUND_RED | FOREGROUND_BLUE,					// MAGENTA
	FOREGROUND_GREEN | FOREGROUND_BLUE,                 // CYAN
	FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, // GREY
	// YELLOW
	FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	// RED_BOLD
	FOREGROUND_RED | FOREGROUND_INTENSITY,
	// GREEN_BOLD
	FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	FOREGROUND_BLUE | FOREGROUND_INTENSITY,             // BLUE_BOLD
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
static uint8_t UnixColorFG[CC_COLOR_MAX] =
{
	FG_BLACK, FG_RED, FG_GREEN, FG_BROWN, FG_BLUE,
	FG_MAGENTA, FG_CYAN, FG_WHITE, FG_YELLOW,
	// 高亮显示
	FG_RED, FG_GREEN, FG_BLUE, FG_MAGENTA, FG_CYAN, FG_WHITE
};
#endif

void Console::setCloseButton(bool enable)
{
#ifdef CU_OS_WIN
	SetConsoleCtrlHandler(ConsoleQuitHandler, enable ? TRUE : FALSE);
#endif
}

void Console::setColor(ColorType color /* = CC_DEFAULT */)
{
#ifdef CU_OS_WIN
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, WinColorFG[color]);
#else
	fprintf(stdout, "\x1b[%d%sm", UnixColorFG[color], (color > CC_YELLOW ? ";1" : ""));
#endif
}

void Console::printf(ColorType color, const char* fmt, ...)
{
	setColor(color);
	va_list va;
	va_start(va, fmt);
	vprintf(fmt, va);
	va_end(va);
}

CU_NS_END