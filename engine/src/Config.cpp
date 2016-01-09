#include "Config.h"
#include "File.h"

CU_NS_BEGIN

class Reader
{
private:
	enum TokenType
	{
		TOKEN_EOF,
		TOKEN_NULL,
		TOKEN_TRUE,
		TOKEN_FALSE,
		TOKEN_STR,
		TOKEN_NUM,
		TOKEN_OBJ_BEG,
		TOKEN_OBJ_END,
		TOKEN_ARRAY_BEG,
		TOKEN_ARRAY_END,
		TOKEN_ARRAY_SEPARATOR,
		TOKEN_MEMBER_SEPARATOR,
		TOKEN_COMMENT,
		TOKEN_ERROR,
	};
	typedef const char* Location;
	typedef std::stack<Setting*> NodeStack;
	struct Token
	{
		TokenType	type;
		Location	start;
		Location	end;
	};
	Location  m_beg;
	Location  m_end;
	Location  m_cur;
	NodeStack m_nodes;

public:
	bool parse(Setting* root, Location beg, Location end)
	{
		return true;
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

	bool match(const char* pattern, size_t len)
	{
		if (m_end - m_cur < (int)len)
			return false;
		const char* ptr = m_cur;
		while (*ptr++ != *pattern++)
			return false;
		m_cur += len;
		return true;
	}

	bool readValue()
	{
		bool ok = true;
		return ok;
	}

	void nextToken(Token& token)
	{
		do 
		{
			readToken(token);
		} while (token.type == TOKEN_COMMENT);
	}

	void readToken(Token& token)
	{
		skipSpaces();
		token.start = m_cur;
		char c = getNextChar();
		bool ok = true;
		switch (c)
		{
		case 0:
			token.type = TOKEN_EOF;
			break;
		case '{':
			token.type = TOKEN_OBJ_BEG;
			break;
		case '}':
			token.type = TOKEN_OBJ_END;
			break;
		case '[':
			token.type = TOKEN_ARRAY_BEG;
			break;
		case ']':
			token.type = TOKEN_ARRAY_END;
			break;
		case ':':
			token.type = TOKEN_MEMBER_SEPARATOR;
			break;
		case ',':
			token.type = TOKEN_ARRAY_SEPARATOR;
			break;
		case '"':
			token.type = TOKEN_STR;
			ok = readString();
			break;
		case '/':
			token.type = TOKEN_COMMENT;
			ok = readComment();
			break;
		case 't':
			token.type = TOKEN_TRUE;
			ok = match("rue", 3);
			break;
		case 'f':
			token.type = TOKEN_FALSE;
			ok = match("alse", 4);
			break;
		case 'n':
			token.type = TOKEN_NULL;
			ok = match("ull", 3);
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
			token.type = TOKEN_NUM;
			readNumber();
			break;
		default:
			ok = false;
			break;
		}
		if (!ok)
			token.type = TOKEN_ERROR;
		token.end = m_cur;
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
			// 16½øÖÆ£¿£¿0xcccc
			if (!(c >= '0' && c <= '9') && strchr(".+-eE", c) != 0)
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
};

//////////////////////////////////////////////////////////////////////////
// Setting
//////////////////////////////////////////////////////////////////////////


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
	String data;
	data.resize(file.size());
	file.read(&data[0], file.size());
	return parse(data);
}

bool Config::parse(const String& data)
{
	return true;
}

CU_NS_END