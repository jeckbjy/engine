#pragma once
#include "Cute/Foundation.h"
#include "Cute/Traits.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Number
{
public:
	enum Type
	{
		TYPE_NONE,
		TYPE_SINT,
		TYPE_UINT,
		TYPE_FLOAT,
	};

	struct Real
	{
		int type;
		union
		{
			int64	vi;
			uint64	vu;
			double	vd;
		};
	};

	static const unsigned short BASE_DEC = 10;
	static const unsigned short BASE_HEX = 0x10;
	static const unsigned short BASE_OCT = 010;

	static String format(int32	value);
	static String format(int64	value);
    static String format(uint32	value);
	static String format(uint64	value);

	static String format(int32	value, int width);
	static String format(int64	value, int width);
	static String format(uint32	value, int width);
    static String format(uint64	value, int width);

	static String format0(int32	value, int width);
	static String format0(int64	value, int width);
	static String format0(uint32 value, int width);
    static String format0(uint64 value, int width);

	static String formatHex(int32	value, bool prefex = false);
	static String formatHex(int64	value, bool prefex = false);
	static String formatHex(uint32	value, bool prefex = false);
    static String formatHex(uint64	value, bool prefex = false);

	static String formatHex(int32	value, int width, bool prefix = false);
	static String formatHex(int64	value, int width, bool prefix = false);
	static String formatHex(uint32	value, int width, bool prefix = false);
    static String formatHex(uint64	value, int width, bool prefix = false);

	static String format(float value);
	static String format(float value, int precision);
	static String format(float value, int width, int precision);

	static String format(double value);
	static String format(double value, int precision);
	static String format(double value, int width, int precision);

	static String format(const void* ptr);

	static void append(String& str, int32	value);
    static void append(String& str, uint32  value);
	static void append(String& str, int64	value);
	static void append(String& str, uint64	value);

	static void append(String& str, int32	value, int width);
    static void append(String& str, uint32	value, int width);
	static void append(String& str, int64	value, int width);
	static void append(String& str, uint64	value, int width);

	static void append0(String& str, int32	value, int width);
    static void append0(String& str, uint32	value, int width);
	static void append0(String& str, int64  value, int width);
	static void append0(String& str, uint64 value, int width);

	static void appendHex(String& str, int32  value, bool prefex = false);
	static void appendHex(String& str, int64  value, bool prefex = false);
	static void appendHex(String& str, uint32 value, bool prefex = false);
    static void appendHex(String& str, uint64 value, bool prefex = false);

	static void appendHex(String& str, int32  value, int width, bool prefex = false);
	static void appendHex(String& str, int64  value, int width, bool prefex = false);
	static void appendHex(String& str, uint32 value, int width, bool prefex = false);
    static void appendHex(String& str, uint64 value, int width, bool prefex = false);

	static void append(String& str, float value);
	static void append(String& str, float value, int precision);
	static void append(String& str, float value, int width, int precision);

	static void append(String& str, double value);
	static void append(String& str, double value, int precision);
	static void append(String& str, double value, int width, int precision);

	static void append(String& str, const void* ptr);

	static void parse(const String& str, bool&   val);
	static void parse(const String& str, int8&   val);
	static void parse(const String& str, int16&  val);
	static void parse(const String& str, int32&  val);
	static void parse(const String& str, int64&  val);
	static void parse(const String& str, uint8&  val);
	static void parse(const String& str, uint16& val);
	static void parse(const String& str, uint32& val);
	static void parse(const String& str, uint64& val);
	static void parse(const String& str, float&  val);
	static void parse(const String& str, double& val);
	static void parse(const String& str, char&   val);

	// parser
	static int		parse(const String& s);
	static unsigned	parseUnsigned(const String& s, char thSep = ',');
	static int64	parse64(const String& s, char thSep = ',');
	static uint64	parseUnsigned64(const String& s, char thSep = ',');
	static float	parseFloat(const String& s, char thSep = ',', char decSep = '.');
	static double	parseDouble(const String& s, char thSep = ',', char decSep = '.');
	static bool		parseBool(const String& s);

	static unsigned parseHex(const String& s);
	static unsigned parseOct(const String& s);
	static uint64	parseHex64(const String& s);
	static uint64	parseOct64(const String& s);

	static bool tryParse(const String& s, int& value, char thSep = ',');
	static bool tryParseUnsigned(const String& s, unsigned& value, char thSep = ',');
	static bool tryParse64(const String& s, int64& value, char thSep = ',');
	static bool tryParseUnsigned64(const String& s, uint64& value, char thSep = ',');
	static bool tryParseFloat(const String& s, float&  value, char thSep = ',', char decSep = '.');
	static bool tryParseDouble(const String& s, double& value, char thSep = ',', char decSep = '.');
	static bool tryParseBool(const String& s, bool& value);

	static bool tryParseHex(const String& s, unsigned& value);
	static bool tryParseOct(const String& s, unsigned& value);
	static bool tryParseHex64(const String& s, uint64& value);
	static bool tryParseOct64(const String& s, uint64& value);

	static double pow10(int exponent);

	static const char* parseNumber(Real& result, const char* buffer);
};

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
inline void Number::parse(const String& str, bool& val)
{
	val = parseBool(str);
}

inline void Number::parse(const String& str, int8& val)
{
	val = (int8)parse(str);
}

inline void Number::parse(const String& str, int16& val)
{
	val = (int16)parse(str);
}

inline void Number::parse(const String& str, int32& val)
{
	val = (int32)parse(str);
}

inline void Number::parse(const String& str, int64& val)
{
	val = parse64(str);
}

inline void Number::parse(const String& str, uint8& val)
{
	val = (uint8)parseUnsigned(str);
}

inline void Number::parse(const String& str, uint16& val)
{
	val = (uint16)parseUnsigned(str);
}

inline void Number::parse(const String& str, uint32& val)
{
	val = (uint32)parseUnsigned(str);
}

inline void Number::parse(const String& str, uint64& val)
{
	val = parseUnsigned64(str);
}

inline void Number::parse(const String& str, float& val)
{
	val = parseFloat(str);
}

inline void Number::parse(const String& str, double& val)
{
	val = parseDouble(str);
}

inline void Number::parse(const String& str, char& val)
{
	val = (char)parse(str);
}

CUTE_NS_END
