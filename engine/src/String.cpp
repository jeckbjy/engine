//! Text
#include "Cute/String.h"
#include "Cute/Ascii.h"
#include "Cute/Numeric.h"
#include <sstream>
#include <iomanip>

CUTE_NS_BEGIN
// http ://www.cnblogs.com/zyl910/archive/2012/08/08/c99int.html

namespace Helper {
    template<typename T>
    inline String& appendDec(String& str, const T& value, int width, char fill = ' ', char thSep = 0)
    {
        char   buffer[CUTE_MAX_INT_STRING_LEN];
        size_t length = CUTE_MAX_INT_STRING_LEN;
        if(Numeric::formatDec(buffer, length, value, width, fill, thSep))
            str.append(buffer, length);
        
        return str;
    }
    
    template<typename T>
    inline String& appendHex(String& str, const T& value, int width, char fill, bool prefix)
    {
        char   buffer[CUTE_MAX_INT_STRING_LEN];
        size_t length = CUTE_MAX_INT_STRING_LEN;
        if(Numeric::formatDec(buffer, length, value, width, fill, prefix))
            str.append(buffer, length);
        
        return str;
    }
    
    template<typename T>
    inline String& appendOct(String& str, const T& value, int width, char fill, bool prefix)
    {
        char   buffer[CUTE_MAX_INT_STRING_LEN];
        size_t length = CUTE_MAX_INT_STRING_LEN;
        if(Numeric::formatOct(buffer, length, value, width, fill, prefix))
            str.append(buffer, length);
        
        return str;
    }
    
    template<typename T>
    inline String& appendFloat(String& str, const T& value, int width, int precision)
    {
        std::stringstream ss;
        if(width > 0)
            ss.width(width);
        
        if(precision > 0)
            ss << std::setprecision(precision);
        
        str.append(ss.str());
        return str;
    }
    
    template<typename T>
    inline String& appendNumber(String& str, const T& value)
    {
        std::stringstream ss;
        ss << value;
        str.append(ss.str());
        return str;
    }
    
    template<typename T>
    inline String& setNumber(String& str, const T& value)
    {
        str.clear();
        str.appends(value);
        return str;
    }
    
    template<typename T>
    inline bool parse(T& value, const String& str)
    {
        std::stringstream ss(str);
        ss>>value;
        return !ss.fail();
    }
}

//
//
//
String::String()
{
}

String::String(char text, size_t count)
    : BaseString(count, text)
{
}

String::String(const char* text)
    : BaseString(text)
{
}

String::String(const char* text, size_t count)
    : BaseString(text, count)
{
}

String::String(const String& other, size_t start, size_t count)
    : BaseString(other, start, count)
{
}

String::String(const BaseString& other)
    : BaseString(other)
{
}

#if defined(CUTE_CPP11)
String::String(String&& other)
    : BaseString(other)
{
}
#endif

String::String(int value)
{
    *this = value;
}

String& String::operator  =(const String& value)
{
	BaseString::assign(value);
	return *this;
}

String& String::operator  =(bool value)
{
    return Helper::setNumber(*this, value);
}

String& String::operator  =(char value)
{
    return Helper::setNumber(*this, value);
}

String& String::operator  =(int8 value)
{
    return Helper::setNumber(*this, value);
}

String& String::operator  =(int16 value)
{
    return Helper::setNumber(*this, value);
}

String& String::operator  =(int32 value)
{
    return Helper::setNumber(*this, value);
}

String& String::operator  =(int64 value)
{
    return Helper::setNumber(*this, value);
}

String& String::operator  =(uint8 value)
{
    return Helper::setNumber(*this, value);
}

String& String::operator =(uint16 value)
{
    return Helper::setNumber(*this, value);
}

String& String::operator =(uint32 value)
{
    return Helper::setNumber(*this, value);
}

String& String::operator =(uint64 value)
{
    return Helper::setNumber(*this, value);
}

String& String::operator =(float value)
{
    return Helper::setNumber(*this, value);
}

String& String::operator =(double value)
{
    return Helper::setNumber(*this, value);
}

String& String::operator +=(const String& value)
{
    this->append(value.data(), value.length());
    return *this;
}

String& String::operator+=(bool value)
{
    return Helper::appendNumber(*this, value);
}

String& String::operator+=(char value)
{
    this->append(&value, 1);
    return *this;
}

String& String::operator+=(int8 value)
{
    return Helper::appendNumber(*this, value);
}

String& String::operator+=(int16 value)
{
    return Helper::appendNumber(*this, value);
}

String& String::operator+=(int32 value)
{
    return Helper::appendNumber(*this, value);
}

String& String::operator+=(int64 value)
{
    return Helper::appendNumber(*this, value);
}

String& String::operator+=(uint8 value)
{
    return Helper::appendNumber(*this, value);
}

String& String::operator+=(uint16 value)
{
    return Helper::appendNumber(*this, value);
}

String& String::operator+=(uint32 value)
{
    return Helper::appendNumber(*this, value);
}

String& String::operator+=(uint64 value)
{
    return Helper::appendNumber(*this, value);
}

String& String::operator+=(float value)
{
    return Helper::appendNumber(*this, value);
}

String& String::operator+=(double value)
{
    return Helper::appendNumber(*this, value);
}

String& String::appendf(const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    appendf(fmt, va);
    va_end(va);
    return *this;
}

String& String::appendf(const char *fmt, va_list &va)
{
    int count = vsnprintf(0, 0, fmt, va);
    if(count > 0)
    {
        size_t length = this->length();
        this->resize(length + count);
        vsnprintf(&(*this)[length], count, fmt, va);
    }
    return *this;
}

String& String::append(const String& value)
{
    BaseString::append(value);
    return *this;
}

String& String::append(const String &value, size_t start, size_t count)
{
    BaseString::append(value, start, count);
    return *this;
}

String& String::append(const char* value)
{
    BaseString::append(value);
    return *this;
}

String& String::append(const char* value, size_t n)
{
    BaseString::append(value, n);
    return *this;
}

String& String::append(const char* value, size_t start, size_t count)
{
    BaseString::append(value, start, count);
    return *this;
}

String& String::append(char value, size_t n)
{
    BaseString::append(value, n);
    return *this;
}

String& String::appends(float  value, int width, int precision)
{
    return Helper::appendFloat(*this, value, width, precision);
}

String& String::appends(double value, int width, int precision)
{
    return Helper::appendFloat(*this, value, width, precision);
}

String& String::appends(int32  value, int width, char fill, char thSep)
{
    return Helper::appendDec(*this, value, width, fill, thSep);
}

String& String::appends(int64  value, int width, char fill, char thSep)
{
	return Helper::appendDec(*this, value, width, fill, thSep);
}

String& String::appends(uint32 value, int width, char fill, char thSep)
{
	return Helper::appendDec(*this, value, width, fill, thSep);
}

String& String::appends(uint64 value, int width, char fill, char thSep)
{
	return Helper::appendDec(*this, value, width, fill, thSep);
}

String& String::append0(int32  value, int width)
{
	return Helper::appendDec(*this, value, width);
}

String& String::append0(int64  value, int width)
{
	return Helper::appendDec(*this, value, width);
}

String& String::append0(uint32 value, int width)
{
	return Helper::appendDec(*this, value, width);
}

String& String::append0(uint64 value, int width)
{
	return Helper::appendDec(*this, value, width);
}

String& String::appendHex(int32  value, int width, char fill, bool prefix)
{
    return Helper::appendHex(*this, value, width, fill, prefix);
}

String& String::appendHex(int64  value, int width, char fill, bool prefix)
{
    return Helper::appendHex(*this, value, width, fill, prefix);
}

String& String::appendHex(uint32 value, int width, char fill, bool prefix)
{
    return Helper::appendHex(*this, value, width, fill, prefix);
}

String& String::appendHex(uint64 value, int width, char fill, bool prefix)
{
    return Helper::appendHex(*this, value, width, fill, prefix);
}

String& String::appendOct(int32  value, int width, char fill, bool prefix)
{
    return Helper::appendOct(*this, value, width, fill, prefix);
}

String& String::appendOct(int64  value, int width, char fill, bool prefix)
{
    return Helper::appendOct(*this, value, width, fill, prefix);
}

String& String::appendOct(uint32 value, int width, char fill, bool prefix)
{
    return Helper::appendOct(*this, value, width, fill, prefix);
}

String& String::appendOct(uint64 value, int width, char fill, bool prefix)
{
    return Helper::appendOct(*this, value, width, fill, prefix);
}

bool String::parse(bool& value) const
{
    return Helper::parse(value, *this);
}

bool String::parse(char&   value) const
{
    return Helper::parse(value, *this);
}

bool String::parse(int8&   value) const
{
    return Helper::parse(value, *this);
}

bool String::parse(int16&  value) const
{
    return Helper::parse(value, *this);
}

bool String::parse(int32&  value) const
{
    return Helper::parse(value, *this);
}

bool String::parse(int64&  value) const
{
    return Helper::parse(value, *this);
}

bool String::parse(uint8&  value) const
{
    return Helper::parse(value, *this);
}

bool String::parse(uint16& value) const
{
    return Helper::parse(value, *this);
}

bool String::parse(uint32& value) const
{
    return Helper::parse(value, *this);
}

bool String::parse(uint64& value) const
{
    return Helper::parse(value, *this);
}

bool String::parse(float&  value) const
{
    return Helper::parse(value, *this);
}

bool String::parse(double& value) const
{
    return Helper::parse(value, *this);
}

//
//
//
int String::icompare(const String &other)const
{
    ConstIterator itr1 = this->begin();
    ConstIterator end1 = this->end();
    ConstIterator itr2 = other.begin();
    ConstIterator end2 = other.end();

    while (itr1 != end1 && itr2 != end2)
    {
        int rc = Ascii::icompare(*itr1, *itr2);
        if (rc != 0)
            return rc;
        ++itr1; ++itr2;
    }

    if (itr1 == end1)
        return itr2 == end2 ? 0 : -1;
    else
        return 1;
}

bool String::iequals(const String& other) const
{
    if (this->size() != other.size())
        return false;

    if (this->data() == other.data())
        return true;

    return std::equal(other.begin(), other.end(), this->begin(), &Ascii::iequals);
}

bool String::istartsWith(const String& prefix) const
{
    if (this->size() < prefix.size())
        return false;

    return std::equal(prefix.begin(), prefix.end(), this->begin(), &Ascii::iequals);
}

bool String::iendsWith(const String& suffix) const
{
    if (this->size() < suffix.size())
        return false;
    return std::equal(suffix.rbegin(), suffix.rend(), this->rbegin(), &Ascii::iequals);
}

bool String::icontains(char ch, size_t start) const
{
    return indexOf(ch, true, start) != -1;
}

bool String::icontains(const String& other, size_t start) const
{
    return indexOf(other, true, start) != -1;
}

bool String::icontainsAny(const String& other, size_t start) const
{
    return indexOf(other, true, start) != -1;
}

bool String::equals(const String& other) const
{
    if (this->size() != other.size())
        return false;

    if (this->data() == other.data())
        return true;

    return std::equal(other.begin(), other.end(), this->begin());
}

bool String::startsWith(const String& prefix) const
{
    if (this->size() < prefix.size())
        return false;

    return std::equal(prefix.begin(), prefix.end(), this->begin());
}

bool String::endsWith(const String& suffix) const
{
    if (this->size() < suffix.size())
        return false;

    return std::equal(suffix.rbegin(), suffix.rend(), this->rbegin());
}

bool String::contains(char ch, size_t start) const
{
    return indexOf(ch, false, start) != -1;
}

bool String::contains(const String& other, size_t start) const
{
    return indexOf(other, false, start) != -1;
}

bool String::containsAny(const String& other, size_t start) const
{
    return indexOf(other, false, start) != -1;
}

long String::indexOf(char ch, bool ignoreCase, size_t start) const
{
    assert(start <= this->size());
    ConstIterator end = this->end();
    if (!ignoreCase) {
        for (ConstIterator iter = this->begin() + start; iter != end; ++iter) {

            if (*iter == ch)
                return (long)(iter - this->begin());
        }
    } else {
        for (ConstIterator iter = this->begin() + start; iter != end; ++iter) {
            if (Ascii::iequals(*iter, ch))
                return (long)(iter - this->begin());
        }
    }

    return -1;
}

long String::indexOf(const String& other, bool ignoreCase, size_t start) const
{
    assert(start <= this->size());
    if (this->size() < other.size())
        return -1;

    ConstIterator iter = this->begin() + start;
    ConstIterator end = this->end() - other.size();
    if (!ignoreCase) {
        for (; iter != end; ++iter) {
            if (std::equal(other.begin(), other.end(), iter))
                return (long)(iter - this->begin());
        }
    } else {
        for (; iter != end; ++iter) {
            if (std::equal(other.begin(), other.end(), iter, &Ascii::iequals))
                return (long)(iter - this->begin());
        }
    }

    return -1;
}

long String::indexOfAny(const String& other, bool ignoreCase, size_t start) const
{
    assert(start <= this->size());
    if (other.empty())
        return -1;

    if (other.size() == 1)
    {
        char ch = other[0];
        for (ConstIterator iter = this->begin() + start; iter != this->end(); ++iter)
        {
            if (*iter == ch)
                return (long)(iter - this->begin());
        }

        return -1;
    }
    else
    {
        for (ConstIterator iter = this->begin() + start; iter != this->end(); ++iter)
        {
            for (ConstIterator oiter = other.begin(); oiter != other.end(); ++oiter)
            {
                if (*iter == *oiter)
                    return (long)(iter - this->begin());
            }
        }

        return -1;
    }
}

long String::lastIndexOf(char ch, bool ignoreCase, size_t rstart) const
{
    assert(rstart <= this->size());
    ConstReverseIterator end = this->rend();
    if (!ignoreCase) {
        for (ConstReverseIterator iter = this->rbegin() + rstart; iter != end; ++iter) {
            if (*iter == ch)
                return (long)(iter - this->rbegin());
        }
    } else {
        for (ConstReverseIterator iter = this->rbegin() + rstart; iter != end; ++iter) {
            if (Ascii::iequals(*iter, ch))
                return (long)(iter - this->rbegin());
        }
    }

    return -1;
}

long String::lastIndexOf(const String& other, bool ignoreCase, size_t rstart) const
{
    assert(rstart <= this->size());
    if (this->size() < other.size())
        return -1;

    ConstReverseIterator iter = this->rbegin() + rstart;
    ConstReverseIterator end = this->rend() - other.size();
    if (!ignoreCase) {
        for (; iter != end; ++iter) {
            if (std::equal(other.rbegin(), other.rend(), iter))
                return (long)(this->size() - (iter - this->rbegin()) );
        }
    } else {
        for (; iter != end; ++iter) {
            if (std::equal(other.rbegin(), other.rend(), iter, &Ascii::iequals))
                return (long)(this->size() - (iter - this->rbegin()) );
        }
    }

    return -1;
}

long String::lastIndexOfAny(const String& other, bool ignoreCase, size_t rstart) const
{
    assert(rstart <= this->size());
    if (other.empty())
        return -1;

    if (other.size() == 1)
    {
        char ch = other[0];
        for (ConstReverseIterator iter = this->rbegin() + rstart; iter != this->rend(); ++iter)
        {
            if (*iter == ch)
                return (long)( this->size() - (iter - this->rbegin()) );
        }

        return -1;
    }
    else
    {
        for (ConstReverseIterator iter = this->rbegin() + rstart; iter != this->rend(); ++iter)
        {
            for (ConstIterator oiter = other.begin(); oiter != other.end(); ++oiter)
            {
                if (*iter == *oiter)
                    return (long)(this->size() - (iter - this->rbegin()));
            }
        }

        return -1;
    }
}

void String::trim()
{
    if (this->empty())
        return;

    size_t first = 0;
    size_t last = this->size() - 1;

    while (first <= last && Ascii::isSpace(this->at(first))) ++first;
    while (last >= first && Ascii::isSpace(this->at(last))) --last;

    ++last;
    if (last != this->size())
        this->resize(last);

    if (first != 0)
        this->erase(0, first);
}

void String::trimLeft()
{
    iterator it = this->begin();
    iterator end = this->end();

    while (it != end && Ascii::isSpace(*it)) ++it;
    if (it != this->begin())
        this->erase(this->begin(), it);
}

void String::trimRight()
{
    if (this->empty())
        return;

    long last_pos = (int)this->size() - 1;
    long pos = last_pos;

    while (pos >= 0 && Ascii::isSpace(this->at(pos))) --pos;
    if (pos != last_pos)
        this->resize((size_t)(pos + 1));
}

void String::toUpper()
{
    iterator iter = this->begin();
    iterator end = this->end();

    while (iter != end) {
        *iter = (value_type)Ascii::toUpper(*iter);
        ++iter;
    }
}

void String::toLower()
{
    iterator iter = this->begin();
    iterator end = this->end();

    while (iter != end) {
        *iter = (value_type)Ascii::toLower(*iter);
        ++iter;
    }
}

void String::remove(size_t start)
{
    this->erase(start);
}

void String::remove(size_t start, size_t length)
{
    this->erase(start, start + length);
}

void String::remove(const String& word)
{
    size_t pos = 0;
    for (;;)
    {
        pos = find_last_of(word, pos);
        if (pos == npos)
            return;
        this->erase(pos, pos + word.size());
    }
}

long String::removeFirst(const String& word, long offset)
{
    size_t pos = find_first_of(word, offset);
    if (pos != npos)
    {
        this->erase(pos, pos + word.size());
        return (long)pos;
    }

    return -1;
}

void String::replace(const String& from, const String& to)
{
    if (from == to)
        return;
    size_t from_size = from.size();
    size_t to_size = to.size();
    size_t pos = 0;
    for (;;)
    {
        pos = find_first_of(from, pos);
        if (pos == npos)
            break;
        BaseString::replace(pos, from_size, to.data(), to_size);
        pos += from_size;
    }
}

long String::replaceFirst(const String& from, const String& to, long offset)
{
    if (from == to)
        return -1;

    size_t pos = find_first_of(from, offset);
    if (pos == npos)
        return -1;

    //
    BaseString::replace(pos, from.size(), to.data(), to.size());
    return long(pos + from.size());
}

bool String::split(StringArray &tokens, char separator, int options) const
{
    if(empty())
        return false;
    
    bool isTrim         = (options & TOKEN_TRIM) != 0;
    bool isIgnoreEmpty  = (options & TOKEN_IGNORE_EMPTY) != 0;
    bool isLastToken    = false;
    
    ConstIterator itor = begin();
    ConstIterator itEnd = end();
    
    String token;
    
    for(; itor != itEnd; ++itor)
    {
        if(*itor == separator)
        {
            if(isTrim)
                token.trim();
            
            if(!token.empty() || !isIgnoreEmpty)
                tokens.push_back(token);
            
            if(!isIgnoreEmpty)
                isLastToken = true;
            
            token.clear();
        }
        else
        {
            token += *itor;
            isLastToken = false;
        }
    }
   
    if(!token.empty())
    {
        if(isTrim)
            token.trim();
        
        if(!token.empty() || !isIgnoreEmpty)
            tokens.push_back(token);
    }
    else if(isLastToken)
    {
        tokens.push_back(String());
    }
    
    return !tokens.empty();
}

bool String::splitAny(StringArray &tokens, const String &separators, int options) const
{
    if(empty())
        return false;
    
    bool isTrim         = (options & TOKEN_TRIM) != 0;
    bool isIgnoreEmpty  = (options & TOKEN_IGNORE_EMPTY) != 0;
    bool isLastToken    = false;
    
    ConstIterator itor = begin();
    ConstIterator itEnd = end();
    
    String token;
    
    for(; itor != itEnd; ++itor)
    {
        if(separators.contains(*itor))
        {
            if(isTrim)
                token.trim();
            
            if(!token.empty() || !isIgnoreEmpty)
                tokens.push_back(token);
            
            if(!isIgnoreEmpty)
                isLastToken = true;
            
            token.clear();
        }
        else
        {
            token += *itor;
            isLastToken = false;
        }
    }
    
    if(!token.empty())
    {
        if(isTrim)
            token.trim();
        
        if(!token.empty() || !isIgnoreEmpty)
            tokens.push_back(token);
    }
    else if(isLastToken)
    {
        tokens.push_back(String());
    }
    
    return !tokens.empty();
}

CUTE_NS_END
