#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

//https://github.com/gallexme/Captain-Hook/blob/master/CaptainHook/Utility/Console.h
//http://bluesock.org/~willg/dev/ansi.html
//https://github.com/tapio/rlutil/blob/master/rlutil.h
class CUTE_CORE_API Console
{
public:
	enum Color
	{
		BLACK,
		RED,
		GREEN,
		BROWN,
		BLUE,
		MAGENTA,
		CYAN,
        WHITE,
        YELLOW,
        // Light Color start
        LRED,
        LGREEN,
        LBLUE,
        LMAGENTA,
        LCYAN,
        LWHITE,
        
//		GRAY,
//		DARKGRAY,
//		LIGHTRED,
//		LIGHTGREEN,
//		YELLOW,
//		LIGHTBLUE,
//		LIGHTMAGENTA,
//		LIGHTCYAN,
//		WHITE,
		COLOR_MAX,		// 最大颜色

        GRAY = WHITE,
		DEFAULT = WHITE,	// 默认原始颜色
	};

	static void setColor(Color color, bool stdout_stream = true);
	static void resetColor(bool stdout_stream = true);
	static Color parseColor(const String& name);
	static String formatColor(Color colr);
};

CUTE_NS_END
