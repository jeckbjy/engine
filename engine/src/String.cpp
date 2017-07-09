//! Text
#include "Cute/String.h"
#include "Cute/Ascii.h"

CUTE_NS_BEGIN
// http ://www.cnblogs.com/zyl910/archive/2012/08/08/c99int.html

String::String()
{
}

String::String(int8 value)
{
    *this += value;
}

String::String(int16 value)
{
    *this += value;
}

String::String(int32 value)
{
    *this += value;
}

String::String(int64 value)
{
    *this += value;
}

String::String(uint8 value)
{
    *this += value;
}

String::String(uint16 value)
{
    *this += value;
}

String::String(uint32 value)
{
    *this += value;
}

String::String(uint64 value)
{
    *this += value;
}

String::String(float value)
{
    *this += value;
}

String::String(double value)
{
    *this += value;
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

String& String::operator  =(const String& value)
{
	BaseString::assign(value);
	return *this;
}

String& String::operator  =(char value)
{
	this->assign(&value, 1);
	return *this;
}

String& String::operator  =(int8 value)
{
	char buff[20];
	sprintf(buff, "%i", value);
	BaseString::assign(buff);
	return *this;
}

String& String::operator  =(int16 value)
{
	char buff[20];
	sprintf(buff, "%hi", value);
	this->assign(buff);
	return *this;
}

String& String::operator  =(int32 value)
{
	char buff[20];
	sprintf(buff, "%i", value);
	this->assign(buff);
	return *this;
}

String& String::operator  =(int64 value)
{
	char buff[20];
	sprintf(buff, "%I64i", value);
	this->assign(buff);
	return *this;
}

String& String::operator  =(uint8 value)
{
	char buff[20];
	//sprintf(buff, "%"PRIu8, value);
	this->assign(buff);
	return *this;
}

String& String::operator =(uint16 value)
{
	char buff[20];
	//sprintf(buff, "%"PRIu16, value);
	this->assign(buff);
	return *this;
}

String& String::operator =(uint32 value)
{
	char buff[20];
	//sprintf(buff, "%"PRIu32, value);
	this->assign(buff);
	return *this;
}

String& String::operator =(uint64 value)
{
	char buff[20];
	//sprintf(buff, "%"PRIu64, value);
	this->assign(buff);
	return *this;
}

String& String::operator =(float value)
{
	char buff[20];
	sprintf(buff, "%f", value);
	this->assign(buff);
	return *this;
}

String& String::operator =(double value)
{
	char buff[20];
	sprintf(buff, "%f", value);
	this->assign(buff);
	return *this;
}

String& String::operator +=(const String& value)
{
    BaseString::append(value.data(), value.length());
    return *this;
}

String& String::operator+=(int8 value)
{
    char buff[20];
    //sprintf(buff, "%"PRIi8, value);
    this->append(buff);
    return *this;
}

String& String::operator+=(int16 value)
{
    char buff[20];
    //sprintf(buff, "%"PRIi16, value);
    this->append(buff);
    return *this;
}

String& String::operator+=(int32 value)
{
    char buff[20];
    //sprintf(buff, "%"PRIi32, value);
    this->append(buff);
    return *this;
}

String& String::operator+=(int64 value)
{
    char buff[20];
    //sprintf(buff, "%"PRIi64, value);
    this->append(buff);
    return *this;
}

String& String::operator+=(uint8 value)
{
    char buff[20];
    //sprintf(buff, "%"PRIu8, value);
    this->append(buff);
    return *this;
}

String& String::operator+=(uint16 value)
{
    char buff[20];
    //sprintf(buff, "%"PRIu16, value);
    this->append(buff);
    return *this;
}

String& String::operator+=(uint32 value)
{
    char buff[20];
    //sprintf(buff, "%"PRIu32, value);
    this->append(buff);
    return *this;
}

String& String::operator+=(uint64 value)
{
    char buff[20];
    //sprintf(buff, "%"PRIu64, value);
    this->append(buff);
    return *this;
}

String& String::operator+=(float value)
{
    char buff[20];
    sprintf(buff, "%f", value);
    this->append(buff);
    return *this;
}

String& String::operator+=(double value)
{
    char buff[20];
    sprintf(buff, "%f", value);
    this->append(buff);
    return *this;
}

String& String::operator+=(char value)
{
    this->append(&value, 1);
    return *this;
}

void String::format(const char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    format(fmt, va);
    va_end(va);
}

void String::format(const char* fmt, va_list& va)
{
    int size = vsnprintf(0, 0, fmt, va);
    if (size > 0)
    {
        this->resize(size + 1);
        size = vsnprintf(&(*this)[0], size + 1, fmt, va);
        if (size == -1)
            this->clear();
        else
            (*this)[size] = '\0';
    }
}

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

// void String::split()
// {

// }

// void String::splitAny()
// {

// }

CUTE_NS_END
