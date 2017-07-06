#include "Cute/Number.h"
#include "Cute/String.h"
#include "Cute/Exception.h"
#include "./conversion/diy-fp.cc"
#include "./conversion/cached-powers.cc"
#include "./conversion/bignum-dtoa.cc"
#include "./conversion/bignum.cc"
#include "./conversion/fast-dtoa.cc"
#include "./conversion/fixed-dtoa.cc"
#include "./conversion/strtod.cc"
#include "./conversion/double-conversion.cc"
#include <memory>
#include <cctype>

CUTE_NS_BEGIN

// binary numbers are supported, thus 64 (bits) + 1 (string terminating zero)
#define CUTE_MAX_INT_STRING_LEN 65
// value from strtod.cc (double_conversion::kMaxSignificantDecimalDigits)
#define CUTE_MAX_FLT_STRING_LEN 780

#define CUTE_FLT_INF "inf"
#define CUTE_FLT_NAN "nan"
#define CUTE_FLT_EXP 'e'

/// Utility char pointer wrapper class.
/// Class ensures increment/decrement remain within boundaries.
class StrPtr
{
public:
	StrPtr(char* ptr, std::size_t offset) 
		: m_beg(ptr), m_cur(ptr), m_end(ptr + offset)
	{
	}

	char*& operator ++ () // prefix
	{
		check(m_cur + 1);
		return ++m_cur;
	}

	char* operator ++ (int) // postfix
	{
		check(m_cur + 1);
		char* tmp = m_cur++;
		return tmp;
	}

	char*& operator -- () // prefix
	{
		check(m_cur - 1);
		return --m_cur;
	}

	char* operator -- (int) // postfix
	{
		check(m_cur - 1);
		char* tmp = m_cur--;
		return tmp;
	}

	char*& operator += (int incr)
	{
		check(m_cur + incr);
		return m_cur += incr;
	}

	char*& operator -= (int decr)
	{
		check(m_cur - decr);
		return m_cur -= decr;
	}

	operator char* () const
	{
		return m_cur;
	}

	std::size_t span() const
	{
		return m_end - m_beg;
	}

private:
	void check(char* ptr)
	{
		if (ptr > m_end) throw RangeException();
	}

	const char* m_beg;
	char*       m_cur;
	const char* m_end;
};

/// Converts integer to string. Numeric bases from binary to hexadecimal are supported.
/// If width is non-zero, it pads the return value with fill character to the specified width.
/// When padding is zero character ('0'), it is prepended to the number itself; all other
/// paddings are prepended to the formatted result with minus sign or base prefix included
/// If prefix is true and base is octal or hexadecimal, respective prefix ('0' for octal, 
/// "0x" for hexadecimal) is prepended. For all other bases, prefix argument is ignored.
/// Formatted string has at least [width] total length.
template <typename T>
bool intToStr(char* result, std::size_t& size, T value, uint16_t base, bool prefix = false, int width = -1, char fill = ' ', char thSep = 0)
{
	if (base < 2 || base > 0x10)
	{
		*result = '\0';
		return false;
	}

	StrPtr ptr(result, size);
	int thCount = 0;
	T tmpVal;
	do
	{
		tmpVal = value;
		value /= base;
		*ptr++ = "FEDCBA9876543210123456789ABCDEF"[15 + (tmpVal - value * base)];
		if (thSep && (base == 10) && (++thCount == 3))
		{
			*ptr++ = thSep;
			thCount = 0;
		}
	} while (value);

	if ('0' == fill)
	{
		if (tmpVal < 0) --width;
		if (prefix && base == 010) --width;
		if (prefix && base == 0x10) width -= 2;
		while ((ptr - result) < width) *ptr++ = fill;
	}

	if (prefix && base == 010)
	{
		*ptr++ = '0';
	}
	else if (prefix && base == 0x10)
	{
		*ptr++ = 'x';
		*ptr++ = '0';
	}

	if (tmpVal < 0) 
		*ptr++ = '-';

	if ('0' != fill)
	{
		while ((ptr - result) < width) *ptr++ = fill;
	}

	size = ptr - result;
	cute_assert_dbg(size <= ptr.span());
	cute_assert_dbg((-1 == width) || (size >= size_t(width)));
	*ptr-- = '\0';

	// inverted
	char* ptrr = result;
	char tmp;
	while (ptrr < ptr)
	{
		tmp = *ptr;
		*ptr-- = *ptrr;
		*ptrr++ = tmp;
	}

	return true;
}

/// Converts integer to string
template <typename T>
inline bool intToStr(String& result, T number, uint16_t base, bool prefix = false, int width = -1, char fill = ' ', char thSep = 0)
{
	char res[CUTE_MAX_INT_STRING_LEN] = { 0 };
	std::size_t size = CUTE_MAX_INT_STRING_LEN;
	bool ret = intToStr(res, size, number, base, prefix, width, fill, thSep);
	result.append(res, size);
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// strToInt
//////////////////////////////////////////////////////////////////////////
/// Converts zero-terminated character array to integer number;
/// Thousand separators are recognized for base10 and current locale;
/// it is silently skipped but not verified for correct positioning.
/// Function returns true if successful. If parsing was unsuccessful,
/// the return value is false with the result value undetermined.
template <typename I>
bool strToInt(const char* pStr, I& result, short base, char thSep = ',')
{
	if (!pStr) 
		return false;

	while (isspace(*pStr)) 
		++pStr;

	if (*pStr == '\0')
		return false;

	short sign = 1;
	if ((*pStr == '-'))
	{
		// Unsigned types can't be negative so abort parsing
		if (std::numeric_limits<I>::min() >= 0) return false;
		sign = -1;
		++pStr;
	}
	else if (*pStr == '+')
	{
		++pStr;
	}

	// parse base
	short tmp_base = 10;
	if (*pStr == '0')
	{
		++pStr;
		if (*pStr == 'x')
		{
			tmp_base = 0x10;
			++pStr;
		}
		else
		{
			tmp_base = 010;
		}
	}

	if (base == 0)
		base = tmp_base;

	// parser states:
	const char STATE_SIGNIFICANT_DIGITS = 1;
	char state = 0;

	result = 0;
	I limitCheck = std::numeric_limits<I>::max() / base;
	for (; *pStr != '\0'; ++pStr)
	{
		switch (*pStr)
		{
		case '0':
			if (state < STATE_SIGNIFICANT_DIGITS) break;

		case '1': case '2': case '3': case '4':
		case '5': case '6': case '7':
			if (state < STATE_SIGNIFICANT_DIGITS) state = STATE_SIGNIFICANT_DIGITS;
			if (result > limitCheck) return false;
			result = result * base + (*pStr - '0');

			break;

		case '8': case '9':
			if ((base == 10) || (base == 0x10))
			{
				if (state < STATE_SIGNIFICANT_DIGITS) state = STATE_SIGNIFICANT_DIGITS;
				if (result > limitCheck) return false;
				result = result * base + (*pStr - '0');
			}
			else return false;

			break;

		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
			if (base != 0x10) return false;
			if (state < STATE_SIGNIFICANT_DIGITS) state = STATE_SIGNIFICANT_DIGITS;
			if (result > limitCheck) return false;
			result = result * base + (10 + *pStr - 'a');

			break;

		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
			if (base != 0x10) return false;
			if (state < STATE_SIGNIFICANT_DIGITS) state = STATE_SIGNIFICANT_DIGITS;
			if (result > limitCheck) return false;
			result = result * base + (10 + *pStr - 'A');

			break;

		case '.':
			if ((base == 10) && (thSep == '.')) break;
			else return false;

		case ',':
			if ((base == 10) && (thSep == ',')) break;
			else return false;

		case ' ':
			if ((base == 10) && (thSep == ' ')) break;
			// fallthrough

		default:
			return false;
		}
	}

	if ((sign < 0) && (base == 10)) 
		result *= sign;

	return true;
}

/// Converts string to integer number;
/// This is a wrapper function, for details see see the
/// bool strToInt(const char*, I&, short, char) implementation.
//template <typename I>
//bool strToInt(const String& str, I& result, short base, char thSep = ',')
//{
//	return strToInt(str.c_str(), result, base, thSep);
//}

//////////////////////////////////////////////////////////////////////////
// float to string
//////////////////////////////////////////////////////////////////////////
/// Pads the string with prefix space and postfix 0.
/// Alternative prefix (e.g. zero instead of space) can be supplied by caller.
/// Used only internally.
void pad(String& str, int precision, int width, char prefix = ' ', char decSep = '.')
{
	// these cases should never happen, if they do, it's a library bug
	cute_assert_dbg(precision > 0);
	cute_assert_dbg(str.length());

	size_t decSepPos = str.find(decSep);
	if (decSepPos == String::npos)
	{
		str.append(1, '.');
		decSepPos = str.size() - 1;
	}

	size_t frac = str.length() - decSepPos - 1;

	size_t ePos = str.find_first_of("eE");
	std::auto_ptr<String> eStr;
	if (ePos != String::npos)
	{
		eStr.reset(new String(str.substr(ePos, std::string::npos)));
		frac -= eStr->length();
		str = str.substr(0, str.length() - eStr->length());
	}

	if (frac != precision)
	{
		if (frac < precision)
		{
			str.append(precision - frac, '0');
		}
		else if ((frac > precision) && (decSepPos != std::string::npos))
		{
			int pos = (int)decSepPos + 1 + precision;
			if (str[pos] >= '5') // we must round up
			{
				char carry = 0;
				if (str[--pos] == '9')
				{
					str[pos] = '0';
					carry = 1;
				}
				else
				{
					++str[pos];
					carry = 0;
				}
				while (--pos >= 0)
				{
					if (str[pos] == decSep) continue;
					if (carry)
					{
						if ((str[pos] + carry) <= '9')
						{
							++str[pos];
							carry = 0;
						}
						else
						{
							str[pos] = '0';
							carry = 1;
						}
					}
				}
				if (carry) str.insert(str.begin(), 1, '1');
			}
			str = str.substr(0, decSepPos + 1 + precision);
		}
	}

	if (eStr.get()) str += *eStr;

	if (width && (str.length() < width)) str.insert(str.begin(), width - str.length(), prefix);
}

/// Inserts thousand separators. Used only internally.
void insertThousandSep(std::string& str, char thSep, char decSep = '.')
{
	cute_assert(decSep != thSep);
	if (str.size() == 0) return;

	std::string::size_type exPos = str.find('e');
	if (exPos == std::string::npos) exPos = str.find('E');
	std::string::size_type decPos = str.find(decSep);
	// there's no rinsert, using forward iterator to go backwards
	std::string::iterator it = str.end();
	if (exPos != std::string::npos) it -= str.size() - exPos;

	if (decPos != std::string::npos)
	{
		while (it != str.begin())
		{
			--it;
			if (*it == decSep) break;
		}
	}
	int thCount = 0;
	if (it == str.end()) --it;
	for (; it != str.begin();)
	{
		std::string::iterator pos = it;
		std::string::value_type chr = *it;
		std::string::value_type prevChr = *--it;

		if (!std::isdigit(chr)) continue;

		if (++thCount == 3 && std::isdigit(prevChr))
			it = str.insert(pos, thSep);

		if (thCount == 3) thCount = 0;
	}
}

void floatToStr(char* buffer, int size, double value, bool single, int precision, int lowDec = 0, int highDec = 0)
{
	using namespace double_conversion;

	if (lowDec == 0 || highDec == 0)
	{
		if (single)
		{
			lowDec  = -std::numeric_limits<float>::digits10;
			highDec =  std::numeric_limits<float>::digits10;
		}
		else
		{
			lowDec = -std::numeric_limits<double>::digits10;
			highDec = std::numeric_limits<double>::digits10;
		}
	}

	StringBuilder builder(buffer, size);
	int flags = DoubleToStringConverter::UNIQUE_ZERO | DoubleToStringConverter::EMIT_POSITIVE_EXPONENT_SIGN;
	DoubleToStringConverter dc(flags, CUTE_FLT_INF, CUTE_FLT_NAN, CUTE_FLT_EXP, lowDec, highDec, 0, 0);

	// 整数
	if (precision == 0)
		value = std::floor(value);

	if (precision <= 0)
	{
		if (single)
			dc.ToShortestSingle((float)value, &builder);
		else
			dc.ToShortest(value, &builder);
	}
	else
	{
		dc.ToFixed(value, precision, &builder);
	}

	builder.Finalize();
}

void floatToStr(String& str, double value, bool single, int precision, int width = -1, char thSep = ',', char decSep = '.')
{
	char buffer[CUTE_MAX_FLT_STRING_LEN];
	floatToStr(buffer, CUTE_MAX_FLT_STRING_LEN, value, single, precision);
	str = buffer;

	if (decSep == 0)
		decSep = '.';

	if (decSep != '.')
        str.replace('.', decSep);

	if (thSep)
		insertThousandSep(str, thSep, decSep);

	if (precision > 0 || width > 0)
		pad(str, precision, width, ' ', decSep);
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
bool strToFloat(const String& str, double& result, char decSep, char thSep)
{
	if (str.empty())
		return false;

	// 会有一次拷贝?
	String tmp(str);
    tmp.trim();
    tmp.remove(thSep);
    tmp.replace(decSep, '.');
    tmp.remove('f');

	using namespace double_conversion;
	int processed;
	int flags = StringToDoubleConverter::ALLOW_LEADING_SPACES | StringToDoubleConverter::ALLOW_TRAILING_SPACES;
	StringToDoubleConverter converter(flags, 0.0, Double::NaN(), CUTE_FLT_INF, CUTE_FLT_NAN);
	result = converter.StringToDouble(str.c_str(), (int)str.size(), &processed);

	Double d(result);
	return !d.IsInfinite() && !d.IsNan();
}

//////////////////////////////////////////////////////////////////////////
// format
//////////////////////////////////////////////////////////////////////////
String Number::format(int value)
{
	String result;
	append(result, value);
	return result;
}

String Number::format(int64	value)
{
	String result;
	append(result, value);
	return result;
}

String Number::format(uint64 value)
{
	String result;
	append(result, value);
	return result;
}

String Number::format(int value, int width)
{
	String result;
	append(result, value, width);
	return result;
}

String Number::format(int64 value, int width)
{
	String result;
	append(result, value, width);
	return result;
}

String Number::format(uint64 value, int width)
{
	String result;
	append(result, value, width);
	return result;
}

String Number::format0(int value, int width)
{
	String result;
	append0(result, value, width);
	return result;
}

String Number::format0(int64 value, int width)
{
	String result;
	append0(result, value, width);
	return result;
}

String Number::format0(uint64 value, int width)
{
	String result;
	append0(result, value, width);
	return result;
}

String Number::formatHex(int value, bool prefex /* = false */)
{
	String result;
	appendHex(result, value, prefex);
	return result;
}

String Number::formatHex(int64 value, bool prefex /* = false */)
{
	String result;
	appendHex(result, value, prefex);
	return result;
}

String Number::formatHex(uint64 value, bool prefex /* = false */)
{
	String result;
	appendHex(result, value, prefex);
	return result;
}

String Number::formatHex(int value, int width, bool prefex /* = false */)
{
	String result;
	appendHex(result, value, width, prefex);
	return result;
}

String Number::formatHex(int64 value, int width, bool prefex /* = false */)
{
	String result;
	appendHex(result, value, width, prefex);
	return result;
}

String Number::formatHex(uint64 value, int width, bool prefex /* = false */)
{
	String result;
	appendHex(result, value, width, prefex);
	return result;
}

String Number::format(float value)
{
	String result;
	append(result, value);
	return result;
}

String Number::format(float value, int precision)
{
	String result;
	append(result, value, precision);
	return result;
}

String Number::format(float value, int width, int precision)
{
	String result;
	append(result, value, width, precision);
	return result;
}

String Number::format(double value)
{
	String result;
	append(result, value);
	return result;
}

String Number::format(double value, int precision)
{
	String result;
	append(result, value, precision);
	return result;
}

String Number::format(double value, int width, int precision)
{
	String result;
	append(result, value, width, precision);
	return result;
}

String Number::format(const void* ptr)
{
	String result;
	append(result, ptr);
	return result;
}

//////////////////////////////////////////////////////////////////////////
// append
//////////////////////////////////////////////////////////////////////////
void Number::append(String& str, int value)
{
	intToStr(str, value, BASE_DEC);
}

void Number::append(String& str, int64 value)
{
	intToStr(str, value, BASE_DEC);
}

void Number::append(String& str, uint64 value)
{
	intToStr(str, value, BASE_DEC);
}

void Number::append(String& str, int value, int width)
{
	intToStr(str, value, BASE_DEC, false, width);
}

void Number::append(String& str, int64 value, int width)
{
	intToStr(str, value, BASE_DEC, false, width);
}

void Number::append(String& str, uint64 value, int width)
{
	intToStr(str, value, BASE_DEC, false, width);
}

void Number::append0(String& str, int value, int width)
{
	intToStr(str, value, BASE_DEC, false, width, '0');
}

void Number::append0(String& str, int64 value, int width)
{
	intToStr(str, value, BASE_DEC, false, width, '0');
}

void Number::append0(String& str, uint64 value, int width)
{
	intToStr(str, value, BASE_DEC, false, width, '0');
}

void Number::appendHex(String& str, int value, bool prefex /* = false */)
{
	intToStr(str, value, BASE_HEX, prefex);
}

void Number::appendHex(String& str, int64 value, bool prefex /* = false */)
{
	intToStr(str, value, BASE_HEX, prefex);
}

void Number::appendHex(String& str, uint64 value, bool prefex /* = false */)
{
	intToStr(str, value, BASE_HEX, prefex);
}

void Number::appendHex(String& str, int value, int width, bool prefex /* = false */)
{
	intToStr(str, value, BASE_HEX, prefex, width);
}

void Number::appendHex(String& str, int64 value, int width, bool prefex /* = false */)
{
	intToStr(str, value, BASE_HEX, prefex, width);
}

void Number::appendHex(String& str, uint64 value, int width, bool prefex /* = false */)
{
	intToStr(str, value, BASE_HEX, prefex, width);
}

void Number::append(String& str, float value)
{
	floatToStr(str, (double)value, true, 0);
}

void Number::append(String& str, float value, int precision)
{
	floatToStr(str, (double)value, true, precision);
}

void Number::append(String& str, float value, int width, int precision)
{
	floatToStr(str, (double)value, true, precision, width);
}

void Number::append(String& str, double value)
{
	floatToStr(str, value, false, 0);
}

void Number::append(String& str, double value, int precision)
{
	floatToStr(str, value, false, precision);
}

void Number::append(String& str, double value, int width, int precision)
{
	floatToStr(str, value, false, precision, width);
}

void Number::append(String& str, const void* ptr)
{
	char buffer[24];
#if defined(CUTE_BIT64)
	std::sprintf(buffer, "%016lX", (unsigned long)ptr);
#else
	std::sprintf(buffer, "%08lX", (unsigned long)ptr);
#endif
	str.append(buffer);
}

//////////////////////////////////////////////////////////////////////////
// parse
//////////////////////////////////////////////////////////////////////////
int Number::parse(const String& s)
{
	int result;
	if (tryParse(s, result))
		return result;
	else
		throw SyntaxException("Not a valid integer", s);
}

unsigned Number::parseUnsigned(const String& s, char thSep)
{
	unsigned result;
	if (tryParseUnsigned(s, result, thSep))
		return result;
	else
		throw SyntaxException("Not a valid unsigned integer", s);
}

int64 Number::parse64(const String& s, char thSep)
{
	int64 result;
	if (tryParse64(s, result, thSep))
		return result;
	else
		throw SyntaxException("Not a valid int64", s);
}

uint64 Number::parseUnsigned64(const String& s, char thSep)
{
	uint64 result;
	if (tryParseUnsigned64(s, result, thSep))
		return result;
	else
		throw SyntaxException("Not a valid uint64", s);
}

float Number::parseFloat(const String& s, char thSep, char decSep)
{
	float result;
	if (tryParseFloat(s, result, thSep, decSep))
		return result;
	else
		throw SyntaxException("Not a valid float", s);
}

double Number::parseDouble(const String& s, char thSep, char decSep)
{
	double result;
	if (tryParseDouble(s, result, thSep, decSep))
		return result;
	else
		throw SyntaxException("Not a valid double", s);
}

bool Number::parseBool(const String& s)
{
	bool result;
	if (tryParseBool(s, result))
		return result;
	else
		throw SyntaxException("Not a valid bool number", s);
}

unsigned Number::parseHex(const String& s)
{
	unsigned result;
	if (tryParseHex(s, result))
		return result;
	else
		throw SyntaxException("Not a valid hex decimal integer", s);
}

unsigned Number::parseOct(const String& s)
{
	unsigned result;
	if (tryParseOct(s, result))
		return result;
	else
		throw SyntaxException("Not a valid oct decimal integer", s);
}

uint64 Number::parseHex64(const String& s)
{
	uint64 result;
	if (tryParseHex64(s, result))
		return result;
	else
		throw SyntaxException("Not a valid hex decimal integer", s);
}

uint64 Number::parseOct64(const String& s)
{
	uint64 result;
	if (tryParseOct64(s, result))
		return result;
	else
		throw SyntaxException("Not a valid oct decimal integer", s);
}

bool Number::tryParse(const String& s, int& value, char thSep)
{
	return strToInt(s.c_str(), value, BASE_DEC, thSep);
}

bool Number::tryParseUnsigned(const String& s, unsigned& value, char thSep)
{
	return strToInt(s.c_str(), value, BASE_DEC, thSep);
}

bool Number::tryParse64(const String& s, int64& value, char thSep)
{
	return strToInt(s.c_str(), value, BASE_DEC, thSep);
}

bool Number::tryParseUnsigned64(const String& s, uint64& value, char thSep)
{
	return strToInt(s.c_str(), value, BASE_DEC, thSep);
}

bool Number::tryParseFloat(const String& s, float& value, char thSep, char decSep)
{
	double tmp;
	bool result = strToFloat(s.c_str(), tmp, decSep, thSep);
	value = (float)tmp;
	return result;
}

bool Number::tryParseDouble(const String& s, double& value, char thSep, char decSep)
{
	return strToFloat(s.c_str(), value, decSep, thSep);
}

bool Number::tryParseBool(const String& s, bool& value)
{
	int n;
	if (tryParse(s, n))
	{
		value = (n != 0);
		return true;
	}

	if (s == "true")
	{
		value = true;
		return true;
	}
	else if (s == "yes")
	{
		value = true;
		return true;
	}
	else if (s == "on")
	{
		value = true;
		return true;
	}

	if (s == "false")
	{
		value = false;
		return true;
	}
	else if (s == "no")
	{
		value = false;
		return true;
	}
	else if (s == "off")
	{
		value = false;
		return true;
	}

	return false;
}

bool Number::tryParseHex(const String& s, unsigned& value)
{
	int offset = 0;
	if (s.size() > 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) offset = 2;
	return strToInt(s.c_str() + offset, value, BASE_HEX);
}

bool Number::tryParseHex64(const String& s, uint64& value)
{
	int offset = 0;
	if (s.size() > 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) offset = 2;
	return strToInt(s.c_str() + offset, value, BASE_HEX);
}

bool Number::tryParseOct(const String& s, unsigned& value)
{
	return strToInt(s.c_str(), value, BASE_OCT);
}

bool Number::tryParseOct64(const String& s, uint64& value)
{
	return strToInt(s.c_str(), value, BASE_OCT);
}

double Number::pow10(int exponent)
{
	if (exponent > 308)
	{
		return std::numeric_limits<double>::infinity();
	}
	else if (exponent < -323)
	{
		return 0.0;
	}
	static const double constants[] = 
	{
		1e-323, 1e-322, 1e-321, 1e-320, 1e-319, 1e-318, 1e-317, 1e-316, 1e-315, 1e-314,
		1e-313, 1e-312, 1e-311, 1e-310, 1e-309, 1e-308, 1e-307, 1e-306, 1e-305, 1e-304,
		1e-303, 1e-302, 1e-301, 1e-300, 1e-299, 1e-298, 1e-297, 1e-296, 1e-295, 1e-294,
		1e-293, 1e-292, 1e-291, 1e-290, 1e-289, 1e-288, 1e-287, 1e-286, 1e-285, 1e-284,
		1e-283, 1e-282, 1e-281, 1e-280, 1e-279, 1e-278, 1e-277, 1e-276, 1e-275, 1e-274,
		1e-273, 1e-272, 1e-271, 1e-270, 1e-269, 1e-268, 1e-267, 1e-266, 1e-265, 1e-264,
		1e-263, 1e-262, 1e-261, 1e-260, 1e-259, 1e-258, 1e-257, 1e-256, 1e-255, 1e-254,
		1e-253, 1e-252, 1e-251, 1e-250, 1e-249, 1e-248, 1e-247, 1e-246, 1e-245, 1e-244,
		1e-243, 1e-242, 1e-241, 1e-240, 1e-239, 1e-238, 1e-237, 1e-236, 1e-235, 1e-234,
		1e-233, 1e-232, 1e-231, 1e-230, 1e-229, 1e-228, 1e-227, 1e-226, 1e-225, 1e-224,
		1e-223, 1e-222, 1e-221, 1e-220, 1e-219, 1e-218, 1e-217, 1e-216, 1e-215, 1e-214,
		1e-213, 1e-212, 1e-211, 1e-210, 1e-209, 1e-208, 1e-207, 1e-206, 1e-205, 1e-204,
		1e-203, 1e-202, 1e-201, 1e-200, 1e-199, 1e-198, 1e-197, 1e-196, 1e-195, 1e-194,
		1e-193, 1e-192, 1e-191, 1e-190, 1e-189, 1e-188, 1e-187, 1e-186, 1e-185, 1e-184,
		1e-183, 1e-182, 1e-181, 1e-180, 1e-179, 1e-178, 1e-177, 1e-176, 1e-175, 1e-174,
		1e-173, 1e-172, 1e-171, 1e-170, 1e-169, 1e-168, 1e-167, 1e-166, 1e-165, 1e-164,
		1e-163, 1e-162, 1e-161, 1e-160, 1e-159, 1e-158, 1e-157, 1e-156, 1e-155, 1e-154,
		1e-153, 1e-152, 1e-151, 1e-150, 1e-149, 1e-148, 1e-147, 1e-146, 1e-145, 1e-144,
		1e-143, 1e-142, 1e-141, 1e-140, 1e-139, 1e-138, 1e-137, 1e-136, 1e-135, 1e-134,
		1e-133, 1e-132, 1e-131, 1e-130, 1e-129, 1e-128, 1e-127, 1e-126, 1e-125, 1e-124,
		1e-123, 1e-122, 1e-121, 1e-120, 1e-119, 1e-118, 1e-117, 1e-116, 1e-115, 1e-114,
		1e-113, 1e-112, 1e-111, 1e-110, 1e-109, 1e-108, 1e-107, 1e-106, 1e-105, 1e-104,
		1e-103, 1e-102, 1e-101, 1e-100, 1e-99, 1e-98, 1e-97, 1e-96, 1e-95, 1e-94, 1e-93,
		1e-92, 1e-91, 1e-90, 1e-89, 1e-88, 1e-87, 1e-86, 1e-85, 1e-84, 1e-83, 1e-82, 1e-81,
		1e-80, 1e-79, 1e-78, 1e-77, 1e-76, 1e-75, 1e-74, 1e-73, 1e-72, 1e-71, 1e-70, 1e-69,
		1e-68, 1e-67, 1e-66, 1e-65, 1e-64, 1e-63, 1e-62, 1e-61, 1e-60, 1e-59, 1e-58, 1e-57,
		1e-56, 1e-55, 1e-54, 1e-53, 1e-52, 1e-51, 1e-50, 1e-49, 1e-48, 1e-47, 1e-46, 1e-45,
		1e-44, 1e-43, 1e-42, 1e-41, 1e-40, 1e-39, 1e-38, 1e-37, 1e-36, 1e-35, 1e-34, 1e-33,
		1e-32, 1e-31, 1e-30, 1e-29, 1e-28, 1e-27, 1e-26, 1e-25, 1e-24, 1e-23, 1e-22, 1e-21,
		1e-20, 1e-19, 1e-18, 1e-17, 1e-16, 1e-15, 1e-14, 1e-13, 1e-12, 1e-11, 1e-10, 1e-9,
		1e-8, 1e-7, 1e-6, 1e-5, 1e-4, 1e-3, 1e-2, 1e-1, 1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7,
		1e8, 1e9, 1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19, 1e20, 1e21,
		1e22, 1e23, 1e24, 1e25, 1e26, 1e27, 1e28, 1e29, 1e30, 1e31, 1e32, 1e33, 1e34, 1e35,
		1e36, 1e37, 1e38, 1e39, 1e40, 1e41, 1e42, 1e43, 1e44, 1e45, 1e46, 1e47, 1e48, 1e49,
		1e50, 1e51, 1e52, 1e53, 1e54, 1e55, 1e56, 1e57, 1e58, 1e59, 1e60, 1e61, 1e62, 1e63,
		1e64, 1e65, 1e66, 1e67, 1e68, 1e69, 1e70, 1e71, 1e72, 1e73, 1e74, 1e75, 1e76, 1e77,
		1e78, 1e79, 1e80, 1e81, 1e82, 1e83, 1e84, 1e85, 1e86, 1e87, 1e88, 1e89, 1e90, 1e91,
		1e92, 1e93, 1e94, 1e95, 1e96, 1e97, 1e98, 1e99, 1e100, 1e101, 1e102, 1e103, 1e104,
		1e105, 1e106, 1e107, 1e108, 1e109, 1e110, 1e111, 1e112, 1e113, 1e114, 1e115, 1e116,
		1e117, 1e118, 1e119, 1e120, 1e121, 1e122, 1e123, 1e124, 1e125, 1e126, 1e127, 1e128,
		1e129, 1e130, 1e131, 1e132, 1e133, 1e134, 1e135, 1e136, 1e137, 1e138, 1e139, 1e140,
		1e141, 1e142, 1e143, 1e144, 1e145, 1e146, 1e147, 1e148, 1e149, 1e150, 1e151, 1e152,
		1e153, 1e154, 1e155, 1e156, 1e157, 1e158, 1e159, 1e160, 1e161, 1e162, 1e163, 1e164,
		1e165, 1e166, 1e167, 1e168, 1e169, 1e170, 1e171, 1e172, 1e173, 1e174, 1e175, 1e176,
		1e177, 1e178, 1e179, 1e180, 1e181, 1e182, 1e183, 1e184, 1e185, 1e186, 1e187, 1e188,
		1e189, 1e190, 1e191, 1e192, 1e193, 1e194, 1e195, 1e196, 1e197, 1e198, 1e199, 1e200,
		1e201, 1e202, 1e203, 1e204, 1e205, 1e206, 1e207, 1e208, 1e209, 1e210, 1e211, 1e212,
		1e213, 1e214, 1e215, 1e216, 1e217, 1e218, 1e219, 1e220, 1e221, 1e222, 1e223, 1e224,
		1e225, 1e226, 1e227, 1e228, 1e229, 1e230, 1e231, 1e232, 1e233, 1e234, 1e235, 1e236,
		1e237, 1e238, 1e239, 1e240, 1e241, 1e242, 1e243, 1e244, 1e245, 1e246, 1e247, 1e248,
		1e249, 1e250, 1e251, 1e252, 1e253, 1e254, 1e255, 1e256, 1e257, 1e258, 1e259, 1e260,
		1e261, 1e262, 1e263, 1e264, 1e265, 1e266, 1e267, 1e268, 1e269, 1e270, 1e271, 1e272,
		1e273, 1e274, 1e275, 1e276, 1e277, 1e278, 1e279, 1e280, 1e281, 1e282, 1e283, 1e284,
		1e285, 1e286, 1e287, 1e288, 1e289, 1e290, 1e291, 1e292, 1e293, 1e294, 1e295, 1e296,
		1e297, 1e298, 1e299, 1e300, 1e301, 1e302, 1e303, 1e304, 1e305, 1e306, 1e307, 1e308
	};

	return constants[exponent + 323];
}

const char* Number::parseNumber(Real& result, const char* buffer)
{
	result.type = TYPE_NONE;

	const char* p = buffer;
	if (*p == 0)
		return p;

	// check negative
	bool isNegative = false;
	if ('-' == *p)
	{
		isNegative = true;
		++p;
		if (*p == 0)
			return p;
	}

	bool isDouble = false;
	uint64 integer = 0;
	int    exponent = 0;
	char c;
	char digit;
	// integral part
	while (*p != 0)
	{
		c = *p;
		if (c < '0' || c > '9')
			break;

		++p;
		digit = c - '0';
		integer = integer * 10 + digit;
	}

	// fractional part
	c = *p;
	if (c == '.')
	{
		isDouble = true;
		++p;
		while (*p != 0)
		{
			c = *p;
			if (c < '0' || c > '9')
				break;
			++p;
			digit = c - '0';
			integer = integer * 10 + digit;
			--exponent;
		}
	}

	// exponential part
	if (*p == 'e' || *p == 'E')
	{
		isDouble = true;
		++p;
		bool isNegativeExponent = false;
		// error
		c = *p;
		if (c == 0)
		{
			return p;
		}
		else if (c == '-')
		{
			isNegativeExponent = true;
			++p;
		}
		else if (c == '+')
		{
			++p;
		}

		int exp = 0;
		while (*p != 0)
		{
			c = *p;
			if (c < '0' || c > '9')
				break;
			++p;
			digit = c - '0';
			exp = exp * 10 + digit;
		}

		exponent += (isNegativeExponent ? -exp : exp);
	}

	if (isDouble)
	{
		double d = (double)integer;
		if (isNegative)
			d = -d;
		if (exponent != 0)
			d = d * pow10(exponent);

		result.type = TYPE_FLOAT;
		result.vd = d;
	}
	else if (isNegative)
	{
		result.type = TYPE_SINT;
		result.vi = -(int64)integer;
	}
	else
	{
		result.type = TYPE_UINT;
		result.vu = integer;
	}

	return p;
}

CUTE_NS_END
