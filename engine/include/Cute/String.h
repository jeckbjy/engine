#pragma once
#include "Cute/Types.h"
#include "Cute/Ascii.h"
#include <string>

CUTE_NS_BEGIN

class CUTE_CORE_API String : public std::string
{
	typedef std::string BaseString;
public:
	typedef iterator            Iterator;
	typedef const_iterator      ConstIterator;
	typedef reverse_iterator    ReverseIterator;
	typedef const_reverse_iterator ConstReverseIterator;
	String();
	String(char text, size_t count = 1);
	String(const char* text);
	String(const char* text, size_t maxChars);
	String(const String& other, size_t start = npos, size_t count = npos);
	String(const BaseString& other);
#if defined(CUTE_CPP11)
	String(String&& other);
#endif
	template<typename Iter>
	String(const Iter& begin, const Iter& end): BaseString(begin, end) {}

	String& operator  =(const String& value);
	String& operator  =(char value);
	String& operator  =(int8 value);
	String& operator  =(int16 value);
	String& operator  =(int32 value);
	String& operator  =(int64 value);
	String& operator  =(uint8 value);
	String& operator  =(uint16 value);
	String& operator  =(uint32 value);
	String& operator  =(uint64 value);
	String& operator  =(float value);
	String& operator  =(double value);

	String& operator +=(const String& value);
	String& operator +=(char value);
	String& operator +=(int8 value);
	String& operator +=(int16 value);
	String& operator +=(int32 value);
	String& operator +=(int64 value);
	String& operator +=(uint8 value);
	String& operator +=(uint16 value);
	String& operator +=(uint32 value);
	String& operator +=(uint64 value);
	String& operator +=(float value);
	String& operator +=(double value);

	String& operator <<(const String& value);
	String& operator <<(char value);
	String& operator <<(int8 value);
	String& operator <<(int16 value);
	String& operator <<(int32 value);
	String& operator <<(int64 value);
	String& operator <<(uint8 value);
	String& operator <<(uint16 value);
	String& operator <<(uint32 value);
	String& operator <<(uint64 value);
	String& operator <<(float value);
	String& operator <<(double value);

	void format(const char* fmt, ...);
	void format(const char* fmt, va_list& va);

	char charAt(size_t index) const;
	char lastCharAt(size_t index) const;

//    int  compare(const String& other) const;
	bool equals(const String& other) const;
	bool startsWith(const String& prefix) const;
	bool endsWith(const String& suffix) const;
	bool contains(char ch, size_t start = 0) const;
	bool contains(const String& other, size_t start = 0) const;
	bool containsAny(const String& other, size_t start = 0) const;

	int  icompare(const String& other) const;
	bool iequals(const String& other) const;
	bool istartsWith(const String& prefix) const;
	bool iendsWith(const String& suffix) const;
	bool icontains(char ch, size_t start = 0) const;
	bool icontains(const String& other, size_t start = 0) const;
	bool icontainsAny(const String& other, size_t start = 0) const;

	long indexOf(char ch, bool ignoreCase = false, size_t start = 0) const;
	long indexOf(const String& other, bool ignoreCase = false, size_t start = 0) const;
	long indexOfAny(const String& other, bool ignoreCase = false, size_t start = 0) const;

	long lastIndexOf(char ch, bool ignoreCase = false, size_t rstart = 0) const;
	long lastIndexOf(const String& other, bool ignoreCase = false, size_t rstart = 0) const;
	long lastIndexOfAny(const String& other, bool ignoreCase = false, size_t rstart = 0) const;

	void trim();
	void trimLeft();
	void trimRight();
	void toUpper();
	void toLower();

	void remove(size_t start);
	void remove(size_t start, size_t length);
	void remove(const String& word);
	long removeFirst(const String& word, long offset = 0);
	void replace(const String& from, const String& to);
	long replaceFirst(const String& from, const String& to, long offset = 0);
	void split();
	void splitAny();
};

//
// inlines
//
inline char String::charAt(size_t index) const
{
	return this->at(index);
}

inline char String::lastCharAt(size_t index) const
{
	return this->at(this->length() - index);
}

//
//
//
inline String& String::operator <<(const String& value) { *this += value; return *this;}
inline String& String::operator <<(char value)   { *this += value; return *this;}
inline String& String::operator <<(int8 value)   { *this += value; return *this;}
inline String& String::operator <<(int16 value)  { *this += value; return *this;}
inline String& String::operator <<(int32 value)  { *this += value; return *this;}
inline String& String::operator <<(int64 value)  { *this += value; return *this;}
inline String& String::operator <<(uint8 value)  { *this += value; return *this;}
inline String& String::operator <<(uint16 value) { *this += value; return *this;}
inline String& String::operator <<(uint32 value) { *this += value; return *this;}
inline String& String::operator <<(uint64 value) { *this += value; return *this;}
inline String& String::operator <<(float value)  { *this += value; return *this;}
inline String& String::operator <<(double value) { *this += value; return *this;}

CUTE_NS_END
