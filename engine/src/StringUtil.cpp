#include "StringUtil.h"

CU_NS_BEGIN

void trim(String& str)
{

}

void trimLeft(String& str)
{

}

void trimRight(String& str)
{

}

void toUpper(String& str)
{

}

void toLower(String& str)
{

}

void startWith(const StringPiece& str, const StringPiece& pattern, bool care /* = true */)
{

}

void endWith(const StringPiece& str, const StringPiece& pattern, bool care /* = true */)
{

}

void replace(String& str, const StringPiece& old_word, const StringPiece& new_word)
{

}

void split(StringList& tokens, const StringPiece& str, const StringPiece& delimiter /* = "|" */, bool compress /* = true */)
{

}

void escape(String& out, const StringPiece& str)
{

}

void unescape(String& out, const StringPiece& str, bool strict /* = true */)
{

}

StringPiece getExtension(const StringPiece& str)
{
	return StringPiece();
}

WString toWString(const StringPiece& str)
{
	return WString();
}

CU_NS_END