#include "Config.h"
#include "File.h"

CU_NS_BEGIN

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

	bool readValue()
	{
		Token token;
		do 
		{
			readToken(token);
		} while (token.type == TOKEN_COMMENT);

		bool ok = true;
		switch (token.type)
		{
		case TOKEN_OBJ_BEG:
			break;
		case TOKEN_ARRAY_BEG:
			break;
		}
		return ok;
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
			// check specil const str
			const char* str_const[3] = { "true", "false", "null" };
			TokenType str_type[3] { TOKEN_TRUE, TOKEN_FALSE, TOKEN_NULL};
			for (int i = 0; i < 3; ++i)
			{
				if (strncmp(token.start, str_const[i], token.end - token.start) == 0)
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
			if (!(c >= '0' && c <= '9') && strchr(".+-eEx", c) != 0)
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

	bool readObject(Token& token)
	{
		// 注意外部已经读取了{
		while (readToken(token))
		{
			if (token.type == TOKEN_COMMENT)
				continue;
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
			// read value
			if (!readValue())
				return false;
			// read comma,最后一个可以没有逗号分隔符
			if (!readToken(token))
				return false;
			// 直接结束
			if (token.type == TOKEN_OBJ_END || token.type == TOKEN_EOF)
				break;
			// must 
			if (token.type != TOKEN_ARRAY_SEP || token.type != TOKEN_COMMENT)
				return false;
		}
	}

	//
	bool parseString(Token& token, String& str)
	{
		return true;
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