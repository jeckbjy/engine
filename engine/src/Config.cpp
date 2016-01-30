#include "Config.h"
#include "File.h"

CU_NS_BEGIN

#define in_str(str, ch) (strchr(str, ch) != 0)
#define not_in_str(str, ch) (strchr(str, ch) == 0)

class Reader
{
private:
	enum TokenType
	{
		TOKEN_NONE,
		TOKEN_EOF,
		TOKEN_COMMENT,

		// const
		TOKEN_NULL,
		TOKEN_TRUE,
		TOKEN_FALSE,

		TOKEN_STR,
		TOKEN_NUM,

		TOKEN_OBJ_BEG,
		TOKEN_OBJ_END,
		TOKEN_OBJ_SEP,	// SEPARATOR

		TOKEN_ARRAY_BEG,
		TOKEN_ARRAY_END,
		TOKEN_ARRAY_SEP,

		TOKEN_ERROR,
	};

	typedef const char* Location;
	struct Token
	{
		TokenType	type;
		Location	start;
		Location	end;
	};
	Location  m_beg;
	Location  m_end;
	Location  m_cur;

public:
	bool parse(Setting* root, Location beg, Location end)
	{
		m_beg = beg;
		m_end = end;
		m_cur = beg;
		// ignore start { and end }
		skipSpaces();
		if (*m_cur == '{')
			++m_cur;
		return readObject(*root);
	}

	char getNextChar()
	{
		if (m_cur == m_end)
			return 0;
		return *m_cur++;
	}

	void skipSpaces()
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

	bool readValue(Setting& value)
	{
		Token token;
		readTokenSkipComments(token);

		bool ok = true;
		switch (token.type)
		{
		case TOKEN_OBJ_BEG:
			ok = readObject(value);
			break;
		case TOKEN_ARRAY_BEG:
			ok = readArray(value);
			break;
		case TOKEN_NUM:
			ok = parseNumber(token, value);
			break;
		case TOKEN_STR:
		{
			String str;
			ok = parseString(token, str);
			if (ok)
				value = str;
		}
		break;
		case TOKEN_TRUE:
			value = true;
			break;
		case TOKEN_FALSE:
			value = false;
			break;
		case TOKEN_NULL:
			value = Setting();
			break;
		default:
			ok = false;
		}
		return ok;
	}

	bool readTokenSkipComments(Token& token)
	{
		do 
		{
			if (!readToken(token))
				return false;
		} while (token.type == TOKEN_COMMENT);
		return true;
	}

	bool readToken(Token& token)
	{
		skipSpaces();
		token.type = TOKEN_NONE;
		token.start = m_cur;
		char c = getNextChar();
		bool ok = true;
		switch (c)
		{
		case 0:
			token.type = TOKEN_EOF;
			break;
		case '/':
			token.type = TOKEN_COMMENT;
			ok = readComment();
			break;
		case '{':
			token.type = TOKEN_OBJ_BEG;
			break;
		case '}':
			token.type = TOKEN_OBJ_END;
			break;
		case '=':
		case ':':
			token.type = TOKEN_OBJ_SEP;
			break;
		case '[':
			token.type = TOKEN_ARRAY_BEG;
			break;
		case ']':
			token.type = TOKEN_ARRAY_END;
			break;
		case ',':
		case ';':
			token.type = TOKEN_ARRAY_SEP;
			break;
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
		case '+':
			token.type = TOKEN_NUM;
			readNumber();
			break;
		case '"':
			token.type = TOKEN_STR;
			ok = readString();
			break;
		default:
		{
			if (!std::isalpha(c))
			{
				ok = false;
				break;
			}
			token.type = TOKEN_STR;

			// 必须是字符，遇到其他字符则结束
			// str or true false null
			while (m_cur != m_end)
			{
				char tmp = *m_cur;
				if (!std::isalpha(tmp))
				{
					break;
				}
				++m_cur;
			}
			Location str_beg = token.start;
			Location str_end = m_cur;
			int len = str_end - str_beg;
			// check specil const str
			const char* str_const[3] = { "true", "false", "null" };
			TokenType str_type[3] { TOKEN_TRUE, TOKEN_FALSE, TOKEN_NULL};
			for (int i = 0; i < 3; ++i)
			{
				if (strncmp(token.start, str_const[i], len) == 0)
				{
					token.type = str_type[i];
					break;
				}
			}
		}
		break;
		}
		if (!ok)
			token.type = TOKEN_ERROR;
		token.end = m_cur;
		return ok;
	}

	bool readString()
	{
		char c;
		while (m_cur != m_end)
		{
			c = getNextChar();
			if (c == '\\')
				getNextChar();
			else if (c == '"')
				break;
		}
		return c == '"';
	}

	void readNumber()
	{
		while (m_cur != m_end)
		{
			char c = *m_cur;
			// 0x
			if (!(c >= '0' && c <= '9') && not_in_str(".+-eEx", c))
				break;
			++m_cur;
		}
	}

	bool readComment()
	{
		//Location commentBegin = m_cur - 1;
		char c = getNextChar();
		bool ok = false;
		if (c == '*')
		{// mutiline
			while (m_cur != m_end)
			{
				char c = getNextChar();
				if (c == '*' && *m_cur == '/')
					break;
			}
			return getNextChar() == '/';
		}
		else
		{
			while (m_cur != m_end)
			{
				char c = getNextChar();
				if (c == '\r' || c == '\n')
					break;
			}
			return true;
		}
		return false;
	}

	bool readObject(Setting& value)
	{
		value.setObject();
		Token token;
		// 注意外部已经读取了{
		while (readTokenSkipComments(token))
		{
			if (token.type == TOKEN_OBJ_END || token.type == TOKEN_EOF)
				break;
			if (token.type != TOKEN_STR)
				return false;
			// 解析str
			String name;
			if (!parseString(token, name))
				return false;
			// read colon
			if (!readToken(token) || token.type != TOKEN_OBJ_SEP)
				return false;
			Setting& sub_value = value[name];
			// read value
			if (!readValue(sub_value))
				return false;
			// read comma,最后一个可以没有逗号分隔符
			if (!readTokenSkipComments(token))
				return false;
			// 直接结束
			if (token.type == TOKEN_OBJ_END || token.type == TOKEN_EOF)
				break;
			// must 
			if (token.type != TOKEN_ARRAY_SEP)
				return false;
		}
		return true;
	}

	bool readArray(Setting& value)
	{
		value.setArray();
		skipSpaces();
		if (*m_cur == ']')
		{
			++m_cur;
			return true;
		}
		Token token;
		for (;;)
		{
			Setting sub_value;
			if (!readValue(sub_value))
				return false;
			value.append(sub_value);
			// read separator
			if (!readTokenSkipComments(token))
				return false;
			if (token.type == TOKEN_ARRAY_END)
				break;
			if (token.type != TOKEN_ARRAY_SEP)
				return false;
		}
		return true;
	}

	bool parseNumber(Token& token, Setting& value)
	{
		bool isHex = false;
		bool isDouble = false;
		if (strncmp(token.start, "0x", 2) == 0)
			isHex = true;
		for (Location inspect = token.start; inspect != token.end; ++inspect)
		{
			if (strchr(".eE+", *inspect) != 0 || *inspect == '-' && inspect != token.start)
			{
				isDouble = true;
				break;
			}
		}
		if (isHex && isDouble)
			return false;
		int count;
		String buffer(token.start, token.end);
		if (isDouble)
		{
			double data = 0;
			count = sscanf(buffer.c_str(), "%lf", &data);
			if (count != 1)
				return false;
			value = data;
		}
		else
		{
			int64_t data = 0;
#ifdef CU_OS_WIN
			count = sscanf(buffer.c_str(), "%l64i", &data);
#else
			count = sscanf(buffer.c_str(), "%Li", &data);
#endif
			if (count != 1)
				return false;
			value = data;
		}
		return true;
	}

	bool parseString(Token& token, String& str)
	{
		// 需要解析转移字符
		int len = token.end - token.start;
		if (len <= 0)
			return true;
		str.reserve((size_t)len);
		Location cur = token.start;
		Location end = token.end;
		while (cur != end)
		{
			char ch = *cur++;
			// 忽略引号
			if (ch == '"')
				continue;
			if (ch == '\\')
			{
				// next char
				if (cur == end)
					return false;
				ch = *cur++;
				switch (ch)
				{
				case '\\':str += '\\'; break;
				case '"':str += '"'; break;
				case '/':str += '/'; break;
				case 'b':str += '\b'; break;
				case 'f':str += '\f'; break;
				case 'n':str += '\n'; break;
				case 'r':str += '\r'; break;
				case 't':str += '\t'; break;
				case 'u':
				{// utf8 code?
					return false;
				}
				break;
				default:
					return false;
				}
			}
			else
			{
				str += ch;
			}
		}
		return true;
	}
};

//////////////////////////////////////////////////////////////////////////
// Setting
//////////////////////////////////////////////////////////////////////////
Setting::Setting(Type type)
:m_type(type)
{
	if (m_type < T_STR)
	{
		m_ival = 0;
	}
	else
	{
		switch (m_type)
		{
		case T_STR:
			m_pstr = new str_t();
			break;
		case T_VEC:
			m_pvec = new vec_t();
			break;
		case T_OBJ:
			m_pmap = new map_t();
			break;
		}
	}
}

Setting::~Setting()
{
	release();
}

Setting::Setting(const Setting& other)
{
	copy(other);
}

Setting& Setting::operator =(const Setting& other)
{
	if (&other != this)
	{
		release();
		copy(other);
	}
	return *this;
}

Setting& Setting::operator =(bool val)
{
	assert(m_type < T_STR);
	m_type = T_BOL;
	m_bval = val;
	return *this;
}

Setting& Setting::operator =(int64_t val)
{
	assert(m_type < T_STR);
	m_type = T_INT;
	m_ival = val;
	return *this;
}

Setting& Setting::operator =(uint64_t val)
{
	assert(m_type < T_STR);
	m_type = T_INT;
	m_ival = (int64_t)val;
	return *this;
}

Setting& Setting::operator =(double val)
{
	assert(m_type < T_STR);
	m_type = T_NUM;
	m_fval = val;
	return *this;
}

Setting& Setting::operator =(const String& val)
{
	if (m_type != T_STR)
		release();
	m_type = T_STR;
	if (m_pstr == 0)
		m_pstr = new str_t();
	m_pstr->data = val;
	return *this;
}

void Setting::setObject()
{
	release();
	m_type = T_OBJ;
	m_pmap = new map_t();
}

void Setting::setArray()
{
	release();
	m_type = T_VEC;
	m_pvec = new vec_t();
}

void Setting::swap(Setting& other)
{
	Type type = m_type;
	m_type = other.m_type;
	other.m_type = type;
	std::swap(m_ival, other.m_ival);
}

void Setting::copy(const Setting& other)
{
	m_type = other.m_type;
	m_ival = other.m_ival;
	if (m_type >= T_STR && m_pstr != 0)
	{
		switch (m_type)
		{
		case T_STR:
			++m_pstr->refs;
			break;
		case T_VEC:
			++m_pvec->refs;
		case T_OBJ:
			++m_pmap->refs;
		}
	}
}

void Setting::release()
{
	if (m_type >= T_STR && m_pstr != 0)
	{
		switch (m_type)
		{
		case T_STR:
			if (--m_pstr->refs <= 0)
				delete m_pstr;
			break;
		case T_VEC:
			if (--m_pvec->refs <= 0)
				delete m_pvec;
			break;
		case T_OBJ:
			if (--m_pmap->refs <= 0)
				delete m_pmap;
			break;
		}
	}
}

size_t Setting::size() const
{
	if (m_type < T_STR || m_ival == 0)
		return 0;
	switch (m_type)
	{
	case T_STR:
		return m_pstr->data.size();
	case T_VEC:
		return m_pvec->data.size();
	case T_OBJ:
		return m_pmap->data.size();
	}
	return 0;
}

bool Setting::empty() const
{
	if (m_type < T_STR || m_ival == 0)
		return true;
	switch (m_type)
	{
	case T_STR:
		return m_pstr->data.empty();
	case T_VEC:
		return m_pvec->data.empty();
	case T_OBJ:
		return m_pmap->data.empty();
	}
	return false;
}

void Setting::clear()
{
	if (m_ival == 0)
		return;
	switch (m_type)
	{
	case T_STR:
		m_pstr->data.clear();
		break;
	case T_VEC:
		m_pvec->data.clear();
		break;
	case T_OBJ:
		m_pmap->data.clear();
		break;
	}
}

void Setting::resize(size_t len)
{
	if (m_type == T_NIL)
		*this = Setting(T_VEC);
	if (m_type != T_VEC)
		return;
	if (m_pvec == 0)
		m_pvec = new vec_t();
	m_pvec->data.resize(len);
}

void Setting::append(const Setting& value)
{
	if (m_type == T_NIL)
		*this = Setting(T_VEC);
	if (m_type != T_VEC)
		return;
	if (m_pvec == 0)
		m_pvec = new vec_t();
	m_pvec->data.push_back(value);
}

bool Setting::contain(const String& key) const
{
	if (m_type != T_OBJ || m_pmap == 0)
		return false;
	return m_pmap->data.find(key) != m_pmap->data.end();
}

Setting& Setting::get(size_t index)
{
	assert(m_type == T_NIL || m_type == T_VEC);
	if (m_type == T_NIL)
		*this = Setting(T_VEC);
	if (m_pvec == 0)
		m_pvec = new vec_t();
	if (index >= m_pvec->data.size())
		m_pvec->data.resize(index + 1);
	return m_pvec->data[index];
}

Setting& Setting::get(const String& key)
{
	assert(m_type == T_NIL || m_type == T_OBJ);
	if (m_type == T_NIL)
		*this = Setting(T_OBJ);
	if (m_pmap == 0)
		m_pmap = new map_t();
	return m_pmap->data[key];
}

const Setting& Setting::get(size_t index) const
{
	assert(m_type == T_VEC && m_pvec != 0);
	return m_pvec->data[index];
}

const Setting& Setting::get(const String& key) const
{
	assert(m_type == T_OBJ && m_pmap != 0);
	return m_pmap->data[key];
}

String Setting::toString() const
{
	std::stringstream ss;
	switch (m_type)
	{
	case T_NIL:ss << "null"; break;
	case T_BOL:ss << m_bval; break;
	case T_INT:ss << m_ival; break;
	case T_NUM:ss << m_fval; break;
	case T_STR:
	{
		if (m_pstr)
			ss << m_pstr->data;
		else
			ss << "";
	}
	break;
	case T_VEC:
	{
		ss << "[" << std::endl;
		if (m_pvec)
		{
			for (size_t i = 0; i < m_pvec->data.size(); ++i)
			{
				ss << m_pvec->data[i].toString() << "," << std::endl;
			}
		}
		ss << "]" << std::endl;
	}
	break;
	case T_OBJ:
	{
		ss << "{" << std::endl;
		if (m_pmap)
		{
			SettingMap& obj = m_pmap->data;
			for (SettingMap::iterator itor = obj.begin(); itor != obj.end(); ++itor)
			{
				ss << itor->first << " : " << itor->second.toString() << "," << std::endl;
			}
		}
		ss << "}" << std::endl;
	}
	break;
	}
	return ss.str();
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
Config::Config()
{

}

Config::~Config()
{

}

bool Config::load(const String& path)
{
	File file(path);
	if (!file.is_open())
		return false;
	size_t len = file.size();
	if (len == 0)
		return true;
	String data;
	data.resize(len);
	file.read(&data[0], (uint)len, 1);
	return parse(data);
}

bool Config::parse(const String& data)
{
	Reader reader;
	if (reader.parse(this, data.c_str(), data.c_str() + data.size()))
		return true;
	return false;
}

CU_NS_END