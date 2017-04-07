//! Config
#include "Cute/JConfig.h"
#include "Cute/Exception.h"
#include "Cute/Number.h"
#include "Cute/Ascii.h"
#include "Cute/File.h"

CUTE_NS_BEGIN

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
class JConfigReader
{
public:
	typedef const char* Location;
	void parse(Variant& root, Location beg, Location end)
	{
		m_line = 0;
		m_beg = beg;
		m_end = end;
		m_cur = beg;

		skipSpace();
		readObject(root);
	}

	void readValue(Variant& value)
	{
		// skip comment
		char c = *m_cur;
		switch (c)
		{
		case '{':
			readObject(value);
			break;
		case '[':
			readArray(value);
			break;
		case '"':
		case '\'':
			readString(value);
			break;
		case '-':
			// must number
			readNumber(value);
			break;
		default:
		{
			// check number，maybe ip xx.xx.xx.xx
			if (Ascii::isDigit(c))
			{

			}
			else if (match4("null"))
			{
				value.reset();
			}
			else if (match4("true"))
			{
				value = true;
			}
			else if (match4("false"))
			{
				value = false;
			}
			else
			{

			}
			break;
		}
		}
	}

	void readObject(Variant& object)
	{
		bool startWith = false;
		char c = *m_cur;
		if (c == '{')
			startWith = true;

		for (;;)
		{
			// check end!
			skipSpace();
			char c = *m_cur;
			if (c == '}')
				break;
			if (c == 0)
			{
				if (startWith)
					error("readObject must match }");
			}

			// 读取key，限制要求必须是字符,数字，或者-和_
			String key;
			readKey(key);

			// colon
			char token = readToken();
			if (token != ':')
				error("readObject colon fail!");

			// value
			Variant value;
			readValue(value);
			object[key] = value;

			// comma,可以没有
			
		}
	}

	void readArray(Variant& value)
	{

	}

	void readString(Variant& value)
	{

	}

	void readString(String& value)
	{

	}

	void readNumber(Variant& value)
	{

	}

	void readKey(String& key)
	{
		skipSpace();
		char c = *m_cur;
		if (c == '"' || c == '\'')
		{// skip
			readString(key);
		}
		else
		{
			key.reserve(32);

			while (m_cur != m_end)
			{
				char c = *m_cur;
				if (Ascii::isAlphaNumeric(c) || c == '-' || c == '_')
				{
					key += c;
				}
				else
				{
					break;
				}
			}
		}
	}

	void readComment()
	{
		// 
		bool multiline = false;
		char c = readChar();
		if (c == '/')
		{
			c = readChar();
			if (c == '*')
				multiline = true;
			else if (c == '/')
				multiline = false;
			else
				error("bad comment!");
		}
		else if (c == '#')
		{
			multiline = false;
		}
		else
			error("bad comment!");

		if (multiline)
		{
			while ((m_cur + 1) < m_end)
			{
				char c = readChar();
				if (c == '*' && *m_cur == '/')
					break;
			}
		}
		else
		{
			while (m_cur != m_end)
			{
				char c = readChar();
				if (c == '\n')
					break;

				if (c == '\r')
				{
					// Consume DOS EOL. It will be normalized in addComment.
					if (m_cur != m_end && *m_cur == '\n')
						readChar();

					// Break on Moc OS 9 EOL.
					break;
				}
			}
		}
	}

	void skipSpace()
	{
		while (m_cur != m_end)
		{
			char c = *m_cur;
			if (c == '\n')
			{
				++m_cur;
				++m_line;
			}
			else if (c == ' ' || c == '\t' || c == '\r')
				++m_cur;
			else
				break;
		}
	}

	char nextToken()
	{
		skipSpace();
		return *m_cur;
	}

	char readToken()
	{
		skipSpace();
		return *m_cur++;
	}

	char readChar()
	{
		return *m_cur++;
	}

	bool match4(Location pattern)
	{
		if (m_end - m_cur < 4)
			return false;

		const char* src = m_cur;
		const char* dst = pattern;
		bool result = (src[0] == dst[0]) && (src[1] == dst[1]) && (src[2] == dst[2]) && (src[3] == dst[3]);
		return result;
	}

	void error(const String& info)
	{
		throw Exception(info + ":" + Number::format(m_line));
	}

private:
	Location m_beg;
	Location m_end;
	Location m_cur;
	int		 m_line;
};

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
Variant JConfig::load(const String& path)
{
	String data;
	File::readAll(path, data);
	return parse(data);
}

Variant JConfig::parse(const String& data)
{
	Variant root;
	JConfigReader reader;
	reader.parse(root, data.c_str(), data.c_str() + data.size());
	return root;
}

CUTE_NS_END
