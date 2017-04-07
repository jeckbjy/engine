//! Config
#include "Cute/Json.h"
#include "Cute/Number.h"
#include <iostream>
#include <iomanip>

CUTE_NS_BEGIN

bool JsonReader::parse(Variant& root, Location beg, Location end)
{
	m_beg = beg;
	m_end = end;
	m_cur = beg;

	if (!readValue(root))
		return false;

	if (!root.isObject() && !root.isArray())
		return false;

	return true;
}

bool JsonReader::readValue(Variant& value)
{
	uint token;
	do 
	{
		token = readToken();
	} while (token == TOKEN_COMMENT);

	bool ok = false;
	switch (token)
	{
	case TOKEN_OBJ_BEG:
		ok = readObject(value);
		break;
	case TOKEN_ARR_BEG:
		ok = readArray(value);
		break;
	case TOKEN_NUM:
		ok = readNumber(value);
		break;
	case TOKEN_STR:
	{
		String str;
		ok = readString(str);
		if (ok)
			value = str;
		break;
	}
	case TOKEN_TRUE:
	{
		ok = true;
		value = true;
		m_cur += 4;
		break;
	}
	case TOKEN_FALSE:
	{
		ok = true;
		value = false;
		m_cur += 5;
		break;
	}
	case TOKEN_NULL:
	{
		ok = true;
		value.reset();
		m_cur += 4;
		break;
	}
	default:
		ok = false;
	}

	return ok;
}

bool JsonReader::readObject(Variant& object)
{
	uint token;
	for (;;)
	{
		// check finish
		skipSpace();
		if (*m_cur == '}')
		{
			++m_cur;
			return true;
		}

		// parse key;
		token = readToken();
		if (token != TOKEN_STR)
			return false;

		String key;
		if (!readString(key))
			return false;

		// parse colon;
		token = readToken();
		if (token != TOKEN_COLON)
			return false;

		// parse value;
		Variant var;
		if (!readValue(var))
			return false;

		object[key] = var;

		// parse comma
		token = readToken();
		if (token == TOKEN_OBJ_END)
			return true;
		else if (token != TOKEN_COMMA)
			return false;
	}
}

bool JsonReader::readArray(Variant& arr)
{
	// [xx,xx]
	uint token;
	for (;;)
	{
		skipSpace();
		if (*m_cur == ']')
		{
			++m_cur;
			break;
		}

		Variant value;
		if (!readValue(value))
			return false;

		arr.append(value);

		// parse comma
		token = readToken();
		if (token == TOKEN_ARR_END)
			return true;
		else if (token != TOKEN_COMMA)
			return false;
	}

	return true;
}

bool JsonReader::readNumber(Variant& value)
{
	// 解析double或者int
	Number::Real result;
	Location pos = Number::parseNumber(result, m_cur);
	if (result.type == Number::TYPE_NONE)
		return false;

	switch (result.type)
	{
	case Number::TYPE_SINT:
		value = result.vi;
		break;
	case Number::TYPE_UINT:
		value = result.vu;
		break;
	case Number::TYPE_FLOAT:
		value = result.vd;
		break;
	}

	m_cur = pos;
	return true;
}

bool JsonReader::readString(String& value)
{
	// "xx"
	bool ok = true;
	Location cur = m_cur;
	while (cur != m_end)
	{
		char c = *cur++;
		if (c == '"')
			break;

		// 发生过错误，忽略错误
		if (!ok)
		{
			if (c == '\\')
				m_cur++;

			continue;
		}

		if (c == '\\')
		{
			if (cur == m_end)
				return false;

			char escape = *cur++;
			ok = append(value, escape);
		}
		else
		{
			value += c;
		}
	}

	return true;
}

bool JsonReader::readComment()
{
	// /**/ or //
	//Location commentBegin = m_cur;
	bool ok = false;
	char c = getNextChar();
	if (c == '*')
	{// mulitline
		while ((m_cur + 1) < m_end)
		{
			char c = getNextChar();
			if (c == '*' && *m_cur == '/')
				break;
		}
	
		ok = (getNextChar() == '/');
	}
	else if (c == '/')
	{// single line
		while (m_cur != m_end)
		{
			char c = getNextChar();
			if (c == '\n')
				break;
			if (c == '\r')
			{
				// Consume DOS EOL. It will be normalized in addComment.
				if (m_cur != m_end && *m_cur == '\n')
					getNextChar();
	
				// Break on Moc OS 9 EOL.
				break;
			}
		}
	
		ok = true;
	}
	
	if (!ok)
		return false;
	
	// add comment
	return true;
}

bool JsonReader::match3(const char* pattern)
{
	// like:true,null
	if (m_end - m_cur < 3)
		return false;

	const char* str = pattern + 1;

	bool result = (m_cur[0] == str[0]) && (m_cur[1] == str[1]) && (m_cur[2] == str[2]);
	m_cur += 3;
	return result;
}

bool JsonReader::match4(const char* pattern)
{
	if (m_end - m_cur < 4)
		return false;

	const char* str = pattern + 1;
	bool result = (m_cur[0] == str[0]) && (m_cur[1] == str[1]) && (m_cur[2] == str[2]) && (m_cur[3] == str[3]);
	m_cur += 4;
	return result;
}

bool JsonReader::append(String& value, char escape)
{
	switch (escape)
	{
	case '"':
		value += '"';
		break;
	case '/':
		value += '/';
		break;
	case '\\':
		value += '\\';
		break;
	case 'b':
		value += '\b';
		break;
	case 'f':
		value += '\f';
		break;
	case 'n':
		value += '\n';
		break;
	case 't':
		value += '\t';
		break;
	case 'u':
	{
		unsigned int cp;
		if (!readCodePoint(cp))
			return false;
		// write utf8
		if (cp <= 0x7f)
		{
			value += cp;
		}
		else if (cp <= 0x7FF)
		{
			value += (char)(0xC0 | (0x1f & (cp >> 6)));
			value += (char)(0x80 | (0x3f & cp));
		}
		else if (cp <= 0xFFFF)
		{
			value += (char)(0xE0 | (0xf & (cp >> 12)));
			value += (char)(0x80 | (0x3f & (cp >> 6)));
			value += (char)(0x80 | (0x3f & cp));
		}
		else if (cp <= 0x10FFFF)
		{
			value += (char)(0xF0 | (0x7  & (cp >> 18)));
			value += (char)(0x80 | (0x3f & (cp >> 12)));
			value += (char)(0x80 | (0x3f & (cp >> 6)));
			value += (char)(0x80 | (0x3f & cp));
		}

		return true;
	}
	default:
		break;
	}

	return true;
}

bool JsonReader::readCodePoint(unsigned int& unicode)
{
	if (!readEscapeSequence(unicode))
		return false;
	if (unicode >= 0xD800 && unicode <= 0xDBFF)
	{
		unsigned int surrogatePair;
		if (*m_cur++ == '\\' && *m_cur++ == 'u')
		{
			if (readEscapeSequence(surrogatePair))
			{
				unicode = 0x10000 + ((unicode & 0x3FF) << 10) + (surrogatePair & 0x3FF);
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool JsonReader::readEscapeSequence(unsigned int& unicode)
{
	unicode = 0;
	for (int index = 0; index < 4; ++index)
	{
		char c = *m_cur++;
		unicode *= 16;
		if (c >= '0' && c <= '9')
			unicode += c - '0';
		else if (c >= 'a' && c <= 'f')
			unicode += c - 'a' + 10;
		else if (c >= 'A' && c <= 'F')
			unicode += c - 'A' + 10;
		else
			return false;
	}

	return true;
}

void JsonReader::skipSpace()
{
	while (m_cur != m_end)
	{
		char c = *m_cur;
		if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
			++m_cur;
		else
			break;
	}
}

uint JsonReader::readToken()
{
	skipSpace();

	char c = getNextChar();

	switch (c)
	{
	case 0:   return TOKEN_EOF;
	case '{': return TOKEN_OBJ_BEG;
	case '}': return TOKEN_OBJ_END;
	case '[': return TOKEN_ARR_BEG;
	case ']': return TOKEN_ARR_END;
	case ',': return TOKEN_COMMA;
	case ':': return TOKEN_COLON;
	case '"': return TOKEN_STR;
	case '/': return TOKEN_COMMENT;
	case 'n': return match3("null") ? TOKEN_NULL : TOKEN_ERROR;
	case 't': return match3("true") ? TOKEN_TRUE : TOKEN_ERROR;
	case 'f': return match4("false") ? TOKEN_FALSE : TOKEN_ERROR;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '-':
		// 回退
		--m_cur;
		return TOKEN_NUM;
	default:
		return TOKEN_ERROR;
	}
}

char JsonReader::getNextChar()
{
	if (m_cur == m_end)
		return 0;
	return *m_cur++;
}

//////////////////////////////////////////////////////////////////////////
// json writer
//////////////////////////////////////////////////////////////////////////
static const char* strnpbrk(char const* s, char const* accept, size_t n)
{
	char const* const end = s + n;
	for (char const* cur = s; cur < end; ++cur)
	{
		int const c = *cur;
		for (char const* a = accept; *a; ++a)
		{
			if (*a == c)
			{
				return cur;
			}
		}
	}
	return NULL;
}

static bool isControlCharacter(char ch)
{
	return ch > 0 && ch <= 0x1F;
}

static bool containsControlCharacter(const char* str, unsigned len)
{
	char const* end = str + len;
	while (end != str)
	{
		if (isControlCharacter(*str) || 0 == *str)
			return true;
		++str;
	}
	return false;
}

static String toQuotedString(const String& str)
{
	if (str.empty())
		return "";

	const char* accept = "\"\\\b\f\n\r\t";
	const char* value = str.c_str();
	size_t length = str.size();

	if (strnpbrk(value, accept, length) == NULL && !containsControlCharacter(value, length))
		return "\"" + str + "\"";
	size_t maxsize = length * 2 + 3;
	String result;
	result.reserve(maxsize);
	result += "\"";

	char const* end = value + length;
	for (const char* c = value; c != end; ++c)
	{
		switch (*c)
		{
		case '\"':
			result += "\\\"";
			break;
		case '\\':
			result += "\\\\";
			break;
		case '\b':
			result += "\\b";
			break;
		case '\f':
			result += "\\f";
			break;
		case '\n':
			result += "\\n";
			break;
		case '\r':
			result += "\\r";
			break;
		case '\t':
			result += "\\t";
			break;
			// case '/':
			// Even though \/ is considered a legal escape in JSON, a bare
			// slash is also legal, so I see no reason to escape it.
			// (I hope I am not misunderstanding something.)
			// blep notes: actually escaping \/ may be useful in javascript to avoid </
			// sequence.
			// Should add a flag to allow this compatibility mode and prevent this
			// sequence from occurring.
		default:
			if ((isControlCharacter(*c)) || (*c == 0))
			{
				std::ostringstream oss;
				oss << "\\u" << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << static_cast<int>(*c);
				result += oss.str();
			}
			else
			{
				result += *c;
			}
			break;
		}
	}
	result += "\"";
	return result;
}

String JsonWriter::write(const Variant& root, bool human)
{
	String doc;
	if (human)
		writeHuman(doc, root);
	else
		writeFast(doc, root);

	return doc;
}

void JsonWriter::writeFast(String& doc, const Variant& value)
{
	switch (value.type())
	{
	case Variant::VAR_NULL:
		doc += "null";
		break;
	case Variant::VAR_INT:
	case Variant::VAR_UINT:
	case Variant::VAR_REAL:
	case Variant::VAR_BOOL:
	case Variant::VAR_DATE:
		doc += value.asString();
		break;
	case Variant::VAR_STR:
		doc += toQuotedString(value.asString());
		break;
	case Variant::VAR_ARRAY:
	{
		size_t length = value.size();
		doc += '[';
		for (size_t index = 0; index < length; ++index)
		{
			if (index > 0)
				doc += ',';
			writeFast(doc, doc[index]);
		}

		doc += ']';
		break;
	}
	case Variant::VAR_OBJ:
	{
		bool first = true;
		doc += "{";
		for (Variant::ConstIterator it = value.begin(); it != value.end(); ++it)
		{
			if (!first)
				doc += ',';
			else
				first = false;
			doc += it->first;
			doc += ":";
			writeFast(doc, it->second);
		}
		doc += "}";
		break;
	}
	default:
		break;
	}
}

void JsonWriter::writeHuman(String& doc, const Variant& value)
{
	switch (value.type())
	{
	case Variant::VAR_NULL:
		break;
	case Variant::VAR_INT:
	case Variant::VAR_UINT:
	case Variant::VAR_REAL:
	case Variant::VAR_BOOL:
	case Variant::VAR_DATE:
		break;
	case Variant::VAR_STR:
		break;
	case Variant::VAR_ARRAY:
		break;
	case Variant::VAR_OBJ:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// json
//////////////////////////////////////////////////////////////////////////
bool Json::parse(Variant& root, const Slice& doc)
{
	JsonReader reader;
	return reader.parse(root, doc.begin(), doc.end());
}

String Json::format(const Variant& root, bool human)
{
	JsonWriter writer;
	return writer.write(root, human);
}

CUTE_NS_END
