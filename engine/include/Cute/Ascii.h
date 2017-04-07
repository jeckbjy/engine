#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

/// This class contains enumerations and static
/// utility functions for dealing with ASCII characters
/// and their properties.
///
/// The classification functions will also work if
/// non-ASCII character codes are passed to them,
/// but classification will only check for
/// ASCII characters.
///
/// This allows the classification methods to be used
/// on the single bytes of a UTF-8 string, without
/// causing assertions or inconsistent results (depending
/// upon the current locale) on bytes outside the ASCII range,
/// as may be produced by Ascii::isSpace(), etc.
class CUTE_CORE_API Ascii
{
public:
	enum CharacterProperties
	{
		ACP_CONTROL = 0x0001,
		ACP_SPACE	= 0x0002,
		ACP_PUNCT	= 0x0004,
		ACP_DIGIT	= 0x0008,
		ACP_HEXDIGIT= 0x0010,
		ACP_ALPHA	= 0x0020,
		ACP_LOWER	= 0x0040,
		ACP_UPPER	= 0x0080,
		ACP_GRAPH	= 0x0100,
		ACP_PRINT	= 0x0200
	};

	/// Return the ASCII character properties for the
	/// character with the given ASCII value.
	///
	/// If the character is outside the ASCII range
	/// (0 .. 127), 0 is returned.
	static int properties(int ch);

	/// Returns true if the given character is
	/// within the ASCII range and has at least one of 
	/// the given properties.
	static bool hasSomeProperties(int ch, int properties);

	/// Returns true if the given character is
	/// within the ASCII range and has all of 
	/// the given properties.
	static bool hasProperties(int ch, int properties);

	static bool isAscii(int ch);
	static bool isSpace(int ch);
	static bool isDigit(int ch);
	static bool isHexDigit(int ch);
	static bool isPunct(int ch);
	static bool isGraph(int ch);
	static bool isAlpha(int ch);
	static bool isAlphaNumeric(int ch);
	static bool isLower(int ch);
	static bool isUpper(int ch);

	static int toLower(int ch);
	static int toUpper(int ch);
private:
	static const int CHARACTER_PROPERTIES[128];
};

//
// inlines
//
inline int Ascii::properties(int ch)
{
	if (isAscii(ch))
		return CHARACTER_PROPERTIES[ch];
	else
		return 0;
}

inline bool Ascii::isAscii(int ch)
{
	return (static_cast<uint32_t>(ch) & 0xFFFFFF80) == 0;
}

inline bool Ascii::hasProperties(int ch, int props)
{
	return (properties(ch) & props) == props;
}

inline bool Ascii::hasSomeProperties(int ch, int props)
{
	return (properties(ch) & props) != 0;
}

inline bool Ascii::isSpace(int ch)
{
	return hasProperties(ch, ACP_SPACE);
}

inline bool Ascii::isDigit(int ch)
{
	return hasProperties(ch, ACP_DIGIT);
}

inline bool Ascii::isHexDigit(int ch)
{
	return hasProperties(ch, ACP_HEXDIGIT);
}

inline bool Ascii::isPunct(int ch)
{
	return hasProperties(ch, ACP_PUNCT);
}

inline bool Ascii::isGraph(int ch)
{
	return hasProperties(ch, ACP_GRAPH);
}

inline bool Ascii::isAlpha(int ch)
{
	return hasProperties(ch, ACP_ALPHA);
}

inline bool Ascii::isAlphaNumeric(int ch)
{
	return hasSomeProperties(ch, ACP_ALPHA | ACP_DIGIT);
}

inline bool Ascii::isLower(int ch)
{
	return hasProperties(ch, ACP_LOWER);
}

inline bool Ascii::isUpper(int ch)
{
	return hasProperties(ch, ACP_UPPER);
}

inline int Ascii::toLower(int ch)
{
	if (isUpper(ch))
		return ch + 32;
	else
		return ch;
}

inline int Ascii::toUpper(int ch)
{
	if (isLower(ch))
		return ch - 32;
	else
		return ch;
}

CUTE_NS_END
