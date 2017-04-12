#pragma once
#include "Cute/Foundation.h"
#include "Cute/DateTime.h"

CUTE_NS_BEGIN

/// This class provides a method for parsing dates and times
/// from strings. All parsing methods do their best to
/// parse a meaningful result, even from malformed input
/// strings.
///
/// The returned DateTime will always contain a time in the same
/// timezone as the time in the string. Call DateTime::makeUTC()
/// with the timeZoneDifferential returned by parse() to convert
/// the DateTime to UTC.
///
/// Note: When parsing a time in 12-hour (AM/PM) format, the hour
/// (%h) must be parsed before the AM/PM designator (%a, %A),
/// otherwise the AM/PM designator will be ignored.
///
/// See the DateTimeFormatter class for a list of supported format specifiers.
/// In addition to the format specifiers supported by DateTimeFormatter, an
/// additional specifier is supported: %r will parse a year given by either
/// two or four digits. Years 69-00 are interpreted in the 20th century
/// (1969-2000), years 01-68 in the 21th century (2001-2068).
///
/// Note that in the current implementation all characters other than format specifiers in 
/// the format string are ignored/not matched against the date/time string. This may
/// lead to non-error results even with nonsense input strings.
/// This may change in a future version to a more strict behavior.
/// If more strict format validation of date/time strings is required, a regular
/// expression could be used for initial validation, before passing the string
/// to DateTimeParser.
class CUTE_CORE_API DateTimeParser
{
public:
	/// Parses a date and time in the given format from the given string.
	/// Throws a SyntaxException if the string cannot be successfully parsed.
	/// Please see DateTimeFormatter::format() for a description of the format string.
	/// Class DateTimeFormat defines format strings for various standard date/time formats.
	static DateTime parse(const String& fmt, const String& str, int& timeZoneDifferential);
	static void parse(const String& fmt, const String& str, DateTime& dateTime, int& timeZoneDifferential);
	static bool tryParse(const String& fmt, const String& str, DateTime& dateTime, int& timeZoneDifferential);

	/// Parses a date and time from the given dateTime string. Before parsing, the method
	/// examines the dateTime string for a known date/time format.
	/// Throws a SyntaxException if the string cannot be successfully parsed.
	/// Please see DateTimeFormatter::format() for a description of the format string.
	/// Class DateTimeFormat defines format strings for various standard date/time formats.
	static DateTime parse(const String& str, int& timeZoneDifferential);
	static void parse(const String& str, DateTime& dateTime, int& timeZoneDifferential);
	static bool tryParse(const String& str, DateTime& dateTime, int& timeZoneDifferential);

	static int parseMonth(String::const_iterator& it, const String::const_iterator& end);
	static int parseDayOfWeek(String::const_iterator& it, const String::const_iterator& end);

protected:
	static int parseTZD(String::const_iterator& it, const String::const_iterator& end);
	static int parseAMPM(String::const_iterator& it, const String::const_iterator& end, int hour);
};

CUTE_NS_END
