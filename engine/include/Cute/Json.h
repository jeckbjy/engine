#pragma once
#include "Cute/Foundation.h"
#include "Cute/Variant.h"
#include "Cute/Stream.h"
#include "Cute/Slice.h"

CUTE_NS_BEGIN

class CUTE_CORE_API JsonReader
{
	typedef const char* Location;
public:
	bool parse(Variant& root, const char* beg, const char* end);

private:
	bool readValue(Variant&  value);
	bool readObject(Variant& value);
	bool readArray(Variant&  value);
	bool readNumber(Variant& value);
	bool readString(String&  value);
	bool readComment();

	bool match3(const char* pattern);
	bool match4(const char* pattern);
	bool append(String& value, char escape);
	bool readCodePoint(unsigned int& unicode);
	bool readEscapeSequence(unsigned int& unicode);

	uint readToken();
	void skipSpace();
	char getNextChar();

private:
	enum TokenType
	{
		TOKEN_NONE,
		TOKEN_EOF,
		TOKEN_OBJ_BEG,
		TOKEN_OBJ_END,
		TOKEN_ARR_BEG,
		TOKEN_ARR_END,
		TOKEN_STR,
		TOKEN_NUM,
		TOKEN_TRUE,
		TOKEN_FALSE,
		TOKEN_NULL,
		TOKEN_COMMA,		// , for obj or array separator
		TOKEN_COLON,		// :
		TOKEN_COMMENT,
		TOKEN_ERROR,
	};

	Location m_beg;
	Location m_end;
	Location m_cur;
};

class CUTE_CORE_API JsonWriter
{
public:
	String write(const Variant& root, bool human = false);

private:
	void writeFast(String& doc, const Variant& value);
	void writeHuman(String& doc, const Variant& value);
};

class CUTE_CORE_API Json
{
public:
	static bool	  parse(Variant& root, const Slice& doc);
	static String format(const Variant& root, bool human);
};

CUTE_NS_END
