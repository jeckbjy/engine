#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class TextEncoding;

/// An unidirectional iterator for iterating over characters in a string.
/// The TextIterator uses a TextEncoding object to
/// work with multi-byte character encodings like UTF-8.
/// Characters are reported in Unicode.
///
/// Example: Count the number of UTF-8 characters in a string.
///
///     UTF8Encoding utf8Encoding;
///     String utf8String("....");
///     TextIterator it(utf8String, utf8Encoding);
///     TextIterator end(utf8String);
///     int n = 0;
///     while (it != end) { ++n; ++it; }
///
/// NOTE: When an UTF-16 encoding is used, surrogate pairs will be
/// reported as two separate characters, due to restrictions of
/// the TextEncoding class.
///
/// For iterating over char buffers, see the TextBufferIterator class.
class CUTE_CORE_API TextIterator
{
	typedef String::const_iterator StringIterator;
public:
	TextIterator();
	TextIterator(const String& str, const TextEncoding& encoding);
	TextIterator(const StringIterator& begin, const StringIterator& end, const TextEncoding& encoding);
	TextIterator(const String& str);
	~TextIterator();

	TextIterator(const TextIterator& it);
	TextIterator& operator = (const TextIterator& it);

	void swap(TextIterator& it);

	TextIterator end() const;

	/// Returns the Unicode value of the current character.
	/// If there is no valid character at the current position,
	/// -1 is returned.
	int operator * () const;

	bool operator ! () const;
	operator bool() const;

	TextIterator& operator ++ ();
	TextIterator operator ++ (int);

	bool operator == (const TextIterator& it) const;
	bool operator != (const TextIterator& it) const;

private:
	const TextEncoding*	m_encoding;
	StringIterator		m_itor;
	StringIterator		m_end;
};

inline bool TextIterator::operator ! () const
{
	return m_itor == m_end;
}

inline TextIterator::operator bool() const
{
	return m_itor != m_end;
}

inline bool TextIterator::operator == (const TextIterator& it) const
{
	return m_itor == it.m_itor;
}

inline bool TextIterator::operator != (const TextIterator& it) const
{
	return m_itor != it.m_itor;
}

inline void swap(TextIterator& it1, TextIterator& it2)
{
	it1.swap(it2);
}

CUTE_NS_END