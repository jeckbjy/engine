#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

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
		GRAY,
		DARKGRAY,
		LIGHTRED,
		LIGHTGREEN,
		YELLOW,
		LIGHTBLUE,
		LIGHTMAGENTA,
		LIGHTCYAN,
		WHITE,
		COLOR_MAX,		// �����ɫ

		DEFAULT = GRAY,	// Ĭ��ԭʼ��ɫ
	};

	static void setColor(Color color = DEFAULT);
	static void resetColor();
	static Color parseColor(const String& name);
	static String formatColor(Color colr);
};

CUTE_NS_END