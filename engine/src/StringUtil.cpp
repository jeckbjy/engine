#include "StringUtil.h"

CU_NS_BEGIN

void trim(String& str)
{

}

void trimLeft(String& str)
{

}

void trimRight(String& str)
{

}

void toUpper(String& str)
{

}

void toLower(String& str)
{

}

void startWith(const StringPiece& str, const StringPiece& pattern, bool care /* = true */)
{

}

void endWith(const StringPiece& str, const StringPiece& pattern, bool care /* = true */)
{

}

void replace(String& str, const StringPiece& old_word, const StringPiece& new_word)
{

}

void split(StringList& tokens, const StringPiece& str, const StringPiece& delimiter /* = "|" */, bool compress /* = true */)
{

}

void escape(String& out, const StringPiece& str)
{

}

void unescape(String& out, const StringPiece& str, bool strict /* = true */)
{

}

StringPiece getExtension(const StringPiece& str)
{
	return StringPiece();
}

WString toWString(const StringPiece& str)
{
	return WString();
}


//namespace StringUtil
//{
//	String toString(const wchar_t* str)
//	{
//		return String();
//	}
//
//	String toString(const WString&)
//	{
//		return String();
//	}
//
//	WString toWString(const char* str)
//	{
//		return WString();
//	}
//	WString toWString(const String& str)
//	{
//		return WString();
//	}
//
//	void vformat(String& result, const char* fmt, va_list& va)
//	{
//		int size = vsnprintf(0, 0, fmt, va);
//		if (size > 0)
//		{
//			result.resize(size + 1);
//			size = vsnprintf(&result[0], size + 1, fmt, va);
//			if (size == -1)
//				result.clear();
//			else
//				result[size] = '\0';
//		}
//	}
//
//	void vformat(String& result, const char* fmt, ...)
//	{
//		va_list va;
//		va_start(va, fmt);
//		vformat(result, fmt, va);
//		va_end(va);
//	}
//
//	String vformat(const char* fmt, ...)
//	{
//		String result;
//		va_list va;
//		va_start(va, fmt);
//		vformat(result, fmt, va);
//		va_end(va);
//		return result;
//	}
//
//	void replace(String& str, const String& old_word, const String& new_word)
//	{
//		for (;;)
//		{
//			String::size_type   pos(0);
//			if ((pos = str.find(old_word)) != String::npos)
//				str.replace(pos, old_word.length(), new_word);
//			else
//				break;
//		}
//	}
//
//	void split(std::vector<String>& tokens, const String& str, const String& delimiter /* = "|" */, bool compress /* = true */)
//	{
//		String::size_type lastPos = 0;
//		String::size_type pos;
//		do
//		{
//			pos = str.find_first_of(delimiter, lastPos);
//			if (pos == String::npos)
//			{
//				if (lastPos != str.length())
//					tokens.push_back(str.substr(lastPos, str.length() - lastPos));
//				break;
//			}
//			tokens.push_back(str.substr(lastPos, pos - lastPos));
//
//			if (compress)
//				lastPos = str.find_first_not_of(delimiter, pos);
//			else
//				lastPos = pos + delimiter.length();
//
//			if (lastPos == String::npos || lastPos == str.length())
//			{// delimiters是最后结尾
//				tokens.push_back("");
//				break;
//			}
//
//		} while (true);
//	}
//
//	void toUpper(String& str)
//	{
//		for (String::iterator it = str.begin(); it != str.end(); ++it)
//			*it = ::toupper(*it);
//	}
//
//	void toLower(String& str)
//	{
//		for (String::iterator it = str.begin(); it != str.end(); ++it)
//			*it = ::tolower(*it);
//	}
//
//	void trimLeft(String& str)
//	{
//		String::iterator it = str.begin();
//		String::iterator end = str.end();
//		while (it != end && ::isspace(*it)) ++it;
//		str.erase(str.begin(), it);
//	}
//
//	void trimRight(String& str)
//	{
//		int pos = int(str.size()) - 1;
//
//		while (pos >= 0 && ::isspace(str[pos])) --pos;
//		str.resize(pos + 1);
//	}
//
//	void trim(String& str)
//	{
//		int first = 0;
//		int last = int(str.size()) - 1;
//
//		while (first <= last && ::isspace(str[first])) ++first;
//		while (last >= first && ::isspace(str[last])) --last;
//
//		str.resize(last + 1);
//		str.erase(0, first);
//	}
//
//	bool startWith(const String& str, const String& pattern, bool lowerCase /* = true */)
//	{
//		size_t len = pattern.size();
//		if (len == 0 || str.size() < len)
//			return false;
//
//		if (lowerCase)
//			return strncasecmp(str.c_str(), pattern.c_str(), len) == 0;
//		else
//			return strncmp(str.c_str(), pattern.c_str(), len) == 0;
//	}
//
//	bool endWith(const String& str, const String& pattern, bool lowerCase /* = true */)
//	{
//		size_t len = pattern.size();
//		if (len == 0 || str.size() < len)
//			return false;
//		const char* ptr = str.c_str() + (str.size() - len);
//		if (lowerCase)
//			return strncasecmp(ptr, pattern.c_str(), len) == 0;
//		else
//			return strncmp(ptr, pattern.c_str(), len) == 0;
//	}
//
//	String fileExtension(const String& str)
//	{
//		size_t pos = str.find_last_of(_T('.'));
//		if (pos != String::npos)
//			return str.substr(pos + 1);
//		return "";
//	}
//}

CU_NS_END