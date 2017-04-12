#pragma once
#include "Cute/Foundation.h"
#include "Cute/DateTime.h"

CUTE_NS_BEGIN

class Timestamp;
class Timespan;

/// This class converts dates and times into strings, supporting a  
/// variety of standard and custom formats.
///
/// There are two kind of static member functions:
///    * format* functions return a String containing
///      the formatted value.
///    * append* functions append the formatted value to
///      an existing string.

/// Formats the given timestamp according to the given format.
/// The format string is used as a template to format the date and
/// is copied character by character except for the following special characters,
/// which are replaced by the corresponding value.
///
///   * %w - abbreviated weekday (Mon, Tue, ...)
///   * %W - full weekday (Monday, Tuesday, ...)
///   * %b - abbreviated month (Jan, Feb, ...)
///   * %B - full month (January, February, ...)
///   * %d - zero-padded day of month (01 .. 31)
///   * %e - day of month (1 .. 31)
///   * %f - space-padded day of month ( 1 .. 31)
///   * %m - zero-padded month (01 .. 12)
///   * %n - month (1 .. 12)
///   * %o - space-padded month ( 1 .. 12)
///   * %y - year without century (70)
///   * %Y - year with century (1970)
///   * %H - hour (00 .. 23)
///   * %h - hour (00 .. 12)
///   * %a - am/pm
///   * %A - AM/PM
///   * %M - minute (00 .. 59)
///   * %S - second (00 .. 59)
///   * %s - seconds and microseconds (equivalent to %S.%F)
///   * %i - millisecond (000 .. 999)
///   * %c - centisecond (0 .. 9)
///   * %F - fractional seconds/microseconds (000000 - 999999)
///   * %z - time zone differential in ISO 8601 format (Z or +NN.NN)
///   * %Z - time zone differential in RFC format (GMT or +NNNN)
///   * %% - percent sign
///
/// Class DateTimeFormat defines format strings for various standard date/time formats.
class CUTE_CORE_API DateTimeFormatter
{
public:
	enum
	{
		UTC = 0xFFFF /// Special value for timeZoneDifferential denoting UTC. 
	};

	static String format(const DateTime& dateTime, const String& fmt, int timeZoneDifferential = UTC);
	static String format(const Timestamp& timestamp, const String& fmt, int timeZoneDifferential = UTC);
	static String format(const Timespan& timespan, const String& fmt = "%dd %H:%M:%S.%i");

	static void append(String& str, const DateTime& dateTime, const String& fmt, int timeZoneDifferential = UTC);
	static void append(String& str, const Timestamp& timestamp, const String& fmt, int timeZoneDifferential = UTC);
	static void append(String& str, const Timespan& timespan, const String& fmt = "%dd %H:%M:%S.%i");

	/// Formats the given timezone differential in ISO format.
	/// If timeZoneDifferential is UTC, "Z" is returned,
	/// otherwise, +HH.MM (or -HH.MM) is returned.
	static String tzdISO(int timeZoneDifferential);

	/// Formats the given timezone differential in RFC format.
	/// If timeZoneDifferential is UTC, "GMT" is returned,
	/// otherwise ++HHMM (or -HHMM) is returned.
	static String tzdRFC(int timeZoneDifferential);

	static void tzdISO(String& str, int timeZoneDifferential);
	static void tzdRFC(String& str, int timeZoneDifferential);
};

//
// inlines
//
inline String DateTimeFormatter::format(const Timestamp& timestamp, const String& fmt, int timeZoneDifferential)
{
	DateTime dateTime(timestamp);
	return format(dateTime, fmt, timeZoneDifferential);
}

inline String DateTimeFormatter::format(const DateTime& dateTime, const String& fmt, int timeZoneDifferential)
{
	String result;
	result.reserve(64);
	append(result, dateTime, fmt, timeZoneDifferential);
	return result;
}

inline String DateTimeFormatter::format(const Timespan& timespan, const String& fmt)
{
	String result;
	result.reserve(32);
	append(result, timespan, fmt);
	return result;
}

inline void DateTimeFormatter::append(String& str, const Timestamp& timestamp, const String& fmt, int timeZoneDifferential)
{
	DateTime dateTime(timestamp);
	append(str, dateTime, fmt, timeZoneDifferential);
}

inline String DateTimeFormatter::tzdISO(int timeZoneDifferential)
{
	String result;
	result.reserve(8);
	tzdISO(result, timeZoneDifferential);
	return result;
}

inline String DateTimeFormatter::tzdRFC(int timeZoneDifferential)
{
	String result;
	result.reserve(8);
	tzdRFC(result, timeZoneDifferential);
	return result;
}

CUTE_NS_END
