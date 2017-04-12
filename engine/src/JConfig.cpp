//! Config
#include "Cute/JConfig.h"
#include "Cute/Exception.h"
#include "Cute/Number.h"
#include "Cute/Ascii.h"
#include "Cute/File.h"

CUTE_NS_BEGIN

//////////////////////////////////////////////////////////////////////////
// JConfigReader
//////////////////////////////////////////////////////////////////////////
void JConfigReader::parse(Variant& root, Location beg, Location end)
{
	m_line = 0;
	m_beg = beg;
	m_end = end;
	m_cur = beg;

	skipSpaceAndComment();
	if (m_cur == m_end)
		return;

	readObject(root, *m_cur == '{');
}

void JConfigReader::readObject(Variant& object, bool startWithBrace)
{
	if (startWithBrace)
		++m_cur;

	char ch;
	for (;;)
	{
		skipSpaceAndComment();
		
		// ��������
		ch = readChar();
		if (ch == '}')
			break;

		// ����ʱ��û��ƥ�䵽}
		if (ch == 0)
		{
			if (startWithBrace)
				error("readObject must match }");
			break;
		}

		// ��ȡkey:�����Ǵ�˫���ŵ��ַ������Կո�,:��=����
		String key;
		readKey(key);

		// read colon
		skipSpaceAndComment();
		ch = readChar();
		if (ch != ':' && ch != '=')
			error("readObject: is not colon!!");

		// read value
		Variant value;
		readValue(value);
		object[key] = value;

		// read comma,����û��
		skipSpaceAndComment();
		ch = *m_cur;
		if (ch == ',')
			++m_cur;
	}
}

void JConfigReader::readArray(Variant& arrays)
{
	assert(*m_cur == '[');
	++m_cur;
	char ch;
	for (;;)
	{
		skipSpaceAndComment();
		ch = readChar();
		if (ch == ']')
			break;

		if (ch == 0)
			error("readArray: must match ]");

		Variant value;
		readValue(value);
		arrays.append(value);

		// read comma;
		skipSpaceAndComment();
		ch = *m_cur;
		if (ch == ',')
			++m_cur;
	}
}

void JConfigReader::readValue(Variant& value)
{
	skipSpaceAndComment();
	char ch = *m_cur;
	switch (ch)
	{
	case '{':
		readObject(value, true);
		break;
	case '[':
		readArray(value);
		break;
	case '"':
		readString(value);
		break;
	case '-':
		readNumber(value);
		break;
	default:
	{
		readDynamic(value);
		break;
	}
	}
}

void JConfigReader::readDynamic(Variant& value)
{
	char ch = *m_cur;
	// �Զ��������:����,��������������null,true,false,datetime(ISO8601),ip��ַ���ַ���
	if (Ascii::isDigit(ch))
	{
		// ���ж��Ƿ���ip:192.168.1.1,
		if (readIP(value))
			return;

		// �ж��Ƿ���datetime
		if (readDateTime(value))
			return;

		// ��ȡ����
		if (readNumber(value))
			return;
	}
	else if (Ascii::isAlpha(ch))
	{
		if (match("null", 4))
		{
			value.reset();
			return;
		}

		if (match("true", 4))
		{
			value = true;
			return;
		}

		if (match("false", 5))
		{
			value = false;
			return;
		}
	}

	// ��ͨ�ַ���
	Location ptr = m_cur;
	while (!isSeparatorKey(*m_cur))
	{
		++ptr;
	}

	if (ptr > m_cur)
	{
		String str;
		str.assign(m_cur, ptr - m_cur);
		value = str;
	}
}

bool JConfigReader::readNumber(Variant& value)
{
	// ����double����int
	Number::Real result;
	Location pos = Number::parseNumber(result, m_cur);
	if (result.type == Number::TYPE_NONE)
	{
		error("readNumber fail!");
		return false;
	}

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

bool JConfigReader::readIP(Variant& value)
{
	bool isOverflow = false;
	int dot = 0; // ��ĸ�����������192.168.1.1
	Location ptr = m_cur;
	char ch;
	int data = 0;
	for (;;)
	{
		if (ptr == m_end)
			break;


		ch = *ptr;

		if (isSeparatorKey(ch))
			break;

		if (ch == '.')
		{
			++dot;
			if (dot > 3)
				return false;
		}
		else if (!Ascii::isDigit(ch))
		{
			return false;
		}
		else
		{
			data = data * 10 + Ascii::toDigit(ch);
			if (data > 255)
				return false;
		}

		++ptr;
	}

	if (dot == 3)
	{
		String str;
		str.assign(m_cur, ptr - m_cur);
		value = str;
		return true;
	}
	else
	{
		return false;
	}
}

bool JConfigReader::readDateTime(Variant& value)
{
	return false;
}

void JConfigReader::readString(Variant& value)
{
	// '"
	String str;
	char endCh = *m_cur;

	++m_cur;

	long offset = 0;
	char buffer[256];
	char ch;
	for (;;)
	{
		ch = readChar();

		if (ch == endCh)
			break;

		if (ch == 0)
		{
			break;
			error("readKey:donnot match end char!!");
		}

		// ����ת��
		if (ch == '\\')
		{
			ch = readChar();

			if (ch == 0)
				error("readKey:escape error!!");
			ch = readEscape(ch);
		}

		buffer[offset++] = ch;
		if (offset == 256)
		{
			str.append(buffer, offset);
			offset = 0;
		}
	}
	
	if (offset > 0)
		str.append(buffer, offset);

	value = str;
}

void JConfigReader::readKey(String& key)
{
	long offset = 0;
	char buffer[256];

	char ch = readChar();
	if (ch == '"' || ch == '\'')
	{// ��ת��
		char endCh = ch;
		for (;;)
		{
			ch = readChar();

			if (ch == endCh)
				break;

			if (ch == 0)
			{
				error("readKey must match '\"'!!");
				break;
			}

			buffer[offset++] = ch;
			if (offset == 256)
			{
				key.append(buffer, offset);
				offset = 0;
			}
		}
	}
	else
	{
		while (ch != ':' && ch != '=' && ch != ' ' && ch != '\t')
		{
			buffer[offset++] = ch;
			ch = readChar();
			if (offset == 256)
			{
				key.append(buffer, offset);
				offset = 0;
			}
		}
	}

	if (offset > 0)
	{
		key.append(buffer, offset);
	}

	if (key.empty())
		error("object key is empty!!");
}

void JConfigReader::skipSpace()
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

void JConfigReader::skipComment()
{
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
			if (c == '\n')
				++m_line;

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
			{
				++m_line;
				break;
			}

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

void JConfigReader::skipSpaceAndComment()
{
	skipSpace();
	skipComment();
	skipSpace();
}

char JConfigReader::readChar()
{
	if (m_cur == m_end)
		return 0;

	return *m_cur++;
}

char JConfigReader::readEscape(char ch)
{
	switch (ch)
	{
	case '"': return '"';
	case '/': return '/';
	case '\\':return '\\';
	case 'b': return '\b';
	case 'f': return '\f';
	case 'n': return '\n';
	case 't': return '\t';
	default:
		error("get escape fail!");
		return ch;
	}
}

bool JConfigReader::match(const char* pattern, int length)
{
	if (m_end - m_cur < length)
		return false;

	for (int i = 0; i < length; ++i)
	{
		if (*(m_cur + i) != *(pattern + i))
			return false;
	}

	// ���һ���ַ���Ҫ�Ƿָ���
	return isSeparatorKey(*(m_cur + length));
}

bool JConfigReader::isSeparatorKey(char ch)
{
	return ch == ',' || ch == '}' || ch == ']' || ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == 0;
}

void JConfigReader::error(const String& info)
{
	throw Exception(info + ":" + Number::format(m_line));
}

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
