#pragma once
#include "Cute/Foundation.h"
#include "Cute/Exception.h"
#include <vector>
#include <cstddef>

CUTE_NS_BEGIN

/// A simple tokenizer that splits a string into
/// tokens, which are separated by separator characters.
/// An iterator is used to iterate over all tokens.
class CUTE_CORE_API StringTokenizer
{
public:
	enum Options
	{
		TOK_IGNORE_EMPTY	= 0x01, /// ignore empty tokens
		TOK_TRIM			= 0x02  /// remove leading and trailing whitespace from tokens
	};

	typedef std::vector<String> TokenVec;
	typedef TokenVec::const_iterator Iterator;

	/// Splits the given string into tokens. The tokens are expected to be
	/// separated by one of the separator characters given in separators.
	/// Additionally, options can be specified:
	///   * TOK_IGNORE_EMPTY: empty tokens are ignored
	///   * TOK_TRIM: trailing and leading whitespace is removed from tokens.
	StringTokenizer(const String& str, const String& separators, int options = 0);
	~StringTokenizer();

	Iterator begin() const;
	Iterator end() const;

	const String& operator [] (std::size_t index) const;
	String& operator [] (std::size_t index);

	bool has(const String& token) const;

	String::size_type find(const String& token, String::size_type pos = 0) const;
	std::size_t replace(const String& oldToken, const String& newToken, String::size_type pos = 0);
	std::size_t count() const;
	std::size_t count(const String& token) const;

private:
	StringTokenizer(const StringTokenizer&);
	StringTokenizer& operator = (const StringTokenizer&);

	void trim(String& token);

	TokenVec m_tokens;
};

//
// inlines
//
inline StringTokenizer::Iterator StringTokenizer::begin() const
{
	return m_tokens.begin();
}

inline StringTokenizer::Iterator StringTokenizer::end() const
{
	return m_tokens.end();
}

inline String& StringTokenizer::operator [] (std::size_t index)
{
	if (index >= m_tokens.size()) throw RangeException();
	return m_tokens[index];
}

inline const String& StringTokenizer::operator [] (std::size_t index) const
{
	if (index >= m_tokens.size()) throw RangeException();
	return m_tokens[index];
}

inline std::size_t StringTokenizer::count() const
{
	return m_tokens.size();
}

CUTE_NS_END
