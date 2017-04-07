#include "Cute/StringTokenizer.h"
#include "Cute/Ascii.h"
#include <algorithm>

CUTE_NS_BEGIN

StringTokenizer::StringTokenizer(const String& str, const String& separators, int options)
{
	String::const_iterator it = str.begin();
	String::const_iterator itEnd = str.end();
	String token;
	bool doTrim = ((options & TOK_TRIM) != 0);
	bool ignoreEmpty = ((options & TOK_IGNORE_EMPTY) != 0);
	bool lastToken = false;

	for (; it != itEnd; ++it)
	{
		if (separators.find(*it) != String::npos)
		{
			if (doTrim) trim(token);
			if (!token.empty() || !ignoreEmpty) m_tokens.push_back(token);
			if (!ignoreEmpty) lastToken = true;
			token.clear();
		}
		else
		{
			token += *it;
			lastToken = false;
		}
	}

	if (!token.empty())
	{
		if (doTrim) trim(token);
		if (!token.empty() || !ignoreEmpty) m_tokens.push_back(token);
	}
	else if (lastToken)
	{
		m_tokens.push_back(String());
	}
}

StringTokenizer::~StringTokenizer()
{
}

void StringTokenizer::trim(String& token)
{
	String::size_type front = 0;
	String::size_type back = 0;
	String::size_type length = token.length();
	String::const_iterator tIt = token.begin();
	String::const_iterator tEnd = token.end();
	for (; tIt != tEnd; ++tIt, ++front)
	{
		if (!Ascii::isSpace(*tIt)) break;
	}
	if (tIt != tEnd)
	{
		String::const_reverse_iterator tRit = token.rbegin();
		String::const_reverse_iterator tRend = token.rend();
		for (; tRit != tRend; ++tRit, ++back)
		{
			if (!Ascii::isSpace(*tRit)) break;
		}
	}
	token = token.substr(front, length - back - front);
}

std::size_t StringTokenizer::count(const String& token) const
{
	std::size_t result = 0;
	TokenVec::const_iterator it = std::find(m_tokens.begin(), m_tokens.end(), token);
	while (it != m_tokens.end())
	{
		result++;
		it = std::find(++it, m_tokens.end(), token);
	}
	return result;
}

String::size_type StringTokenizer::find(const String& token, String::size_type pos) const
{
	TokenVec::const_iterator it = std::find(m_tokens.begin() + pos, m_tokens.end(), token);
	if (it != m_tokens.end())
	{
		return it - m_tokens.begin();
	}
	throw NotFoundException(token);
}

bool StringTokenizer::has(const String& token) const
{
	TokenVec::const_iterator it = std::find(m_tokens.begin(), m_tokens.end(), token);
	return it != m_tokens.end();
}

std::size_t StringTokenizer::replace(const String& oldToken, const String& newToken, String::size_type pos)
{
	std::size_t result = 0;
	TokenVec::iterator it = std::find(m_tokens.begin() + pos, m_tokens.end(), oldToken);
	while (it != m_tokens.end())
	{
		result++;
		*it = newToken;
		it = std::find(++it, m_tokens.end(), oldToken);
	}
	return result;
}

CUTE_NS_END