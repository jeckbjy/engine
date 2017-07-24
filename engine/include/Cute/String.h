#pragma once
#include "Cute/Types.h"
#include "Cute/Ascii.h"
#include <string>

CUTE_NS_BEGIN

// %[flags][width][.precision][length]specifier 
//format: http://www.cplusplus.com/reference/cstdio/printf/
class CUTE_CORE_API String : public std::string
{
	typedef std::string BaseString;
public:
	enum
	{
		TOKEN_IGNORE_EMPTY	= 0x01,
		TOKEN_TRIM			= 0x02,
        TOKEN_ALL           = 0x03,
	};

	typedef Vector<String>          StringArray;
	typedef iterator                Iterator;
	typedef const_iterator          ConstIterator;
	typedef reverse_iterator        ReverseIterator;
	typedef const_reverse_iterator  ConstReverseIterator;

public:
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
	String& operator  =(char   value);
	String& operator  =(int8   value);
	String& operator  =(int16  value);
	String& operator  =(int32  value);
	String& operator  =(int64  value);
	String& operator  =(uint8  value);
	String& operator  =(uint16 value);
	String& operator  =(uint32 value);
	String& operator  =(uint64 value);
	String& operator  =(float  value);
	String& operator  =(double value);

	String& operator +=(const String& value);
	String& operator +=(char   value);
	String& operator +=(int8   value);
	String& operator +=(int16  value);
	String& operator +=(int32  value);
	String& operator +=(int64  value);
	String& operator +=(uint8  value);
	String& operator +=(uint16 value);
	String& operator +=(uint32 value);
	String& operator +=(uint64 value);
	String& operator +=(float  value);
	String& operator +=(double value);
  
    String& append(const String& value);
    String& append(const String& value, size_t start, size_t count);
    String& append(const char* value);
    String& append(const char* value, size_t n);
    String& append(const char* value, size_t start, size_t count);
    String& append(char value, size_t n);
    
    String& appendf(const char* fmt, ...);
    String& appendf(const char* fmt, va_list& va);

    String& appends(float  value, int width = 0, int precision = 0);
	String& appends(double value, int width = 0, int precision = 0);
    
	String& appends(int32  value, int width = 0, char fill=0);
    String& appends(int64  value, int width = 0, char fill=0);
    String& appends(uint32 value, int width = 0, char fill=0);
    String& appends(uint64 value, int width = 0, char fill=0);
    
	String& append0(int32  value, int width);
	String& append0(int64  value, int width);
	String& append0(uint32 value, int width);
	String& append0(uint64 value, int width);
    
	String& appendHex(int32  value, int width = 0, bool prefex = false);
	String& appendHex(int64  value, int width = 0, bool prefex = false);
	String& appendHex(uint32 value, int width = 0, bool prefex = false);
	String& appendHex(uint64 value, int width = 0, bool prefex = false);
    
	String& appendOct(int32  value, int width = 0, bool prefex = false);
	String& appendOct(int64  value, int width = 0, bool prefex = false);
	String& appendOct(uint32 value, int width = 0, bool prefex = false);
	String& appendOct(uint64 value, int width = 0, bool prefex = false);
    
    bool parse(char&   value) const;
    bool parse(int8&   value) const;
    bool parse(int16&  value) const;
    bool parse(int32&  value) const;
    bool parse(int64&  value) const;
    bool parse(uint8&  value) const;
    bool parse(uint16& value) const;
    bool parse(uint32& value) const;
    bool parse(uint64& value) const;
    bool parse(float&  value) const;
    bool parse(double& value) const;
    
    int    toInteger() const;
    uint   toUnsigned() const;
    float  toFloat() const;
    double toDouble() const;
    
    template<typename T>
    T toNumber() const;

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

	bool split(StringArray& tokens, char separator, int options = 0) const;
    bool splitAny(StringArray& tokens, const String& separators, int options = 0) const;
};

typedef Vector<String> StringArray;

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

inline int String::toInteger() const
{
    return toNumber<int>();
}

inline uint String::toUnsigned() const
{
    return toNumber<uint>();
}

inline float String::toFloat() const
{
    return toNumber<float>();
}

inline double String::toDouble() const
{
    return toNumber<double>();
}

template<typename T>
inline T String::toNumber() const
{
    T result;
    if(parse(result))
        return result;
    else
        return (T)0;
}

// 流处理
template<typename T>
inline String& operator <<(String& str,const T& value)
{
    str += value;
    return str;
}

CUTE_NS_END
