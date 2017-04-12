#pragma once
#include "Cute/Foundation.h"
#include "Cute/Timestamp.h"
#include "Cute/Timespan.h"

CUTE_NS_BEGIN

/// This class represents an instant in time, expressed
/// in years, months, days, hours, minutes, seconds
/// and milliseconds based on the Gregorian calendar.
/// The class is mainly useful for conversions between
/// UTC, Julian day and Gregorian calendar dates.
///
/// The date and time stored in a DateTime is always in UTC
/// (Coordinated Universal Time) and thus independent of the 
/// timezone in effect on the system.
///
/// Conversion calculations are based on algorithms
/// collected and described by Peter Baum at
/// http://vsg.cape.com/~pbaum/date/date0.htm
///
/// Internally, this class stores a date/time in two
/// forms (UTC and broken down) for performance reasons. Only use 
/// this class for conversions between date/time representations.
/// Use the Timestamp class for everything else.
///
/// Notes:
///   * Zero is a valid year (in accordance with ISO 8601 and astronomical year numbering)
///   * Year zero (0) is a leap year
///   * Negative years (years preceding 1 BC) are not supported
///
/// For more information, please see:
///   * http://en.wikipedia.org/wiki/Gregorian_Calendar
///   * http://en.wikipedia.org/wiki/Julian_day
///   * http://en.wikipedia.org/wiki/UTC
///   * http://en.wikipedia.org/wiki/ISO_8601

///   * year is from 0 to 9999.
///   * month is from 1 to 12.
///   * day is from 1 to 31.
///   * hour is from 0 to 23.
///   * minute is from 0 to 59.
///   * second is from 0 to 59.
///   * millisecond is from 0 to 999.
///   * microsecond is from 0 to 999.
class CUTE_CORE_API DateTime
{
public:
	/// Symbolic names for month numbers (1 to 12).
	enum Months
	{
		JANUARY = 1,
		FEBRUARY,
		MARCH,
		APRIL,
		MAY,
		JUNE,
		JULY,
		AUGUST,
		SEPTEMBER,
		OCTOBER,
		NOVEMBER,
		DECEMBER
	};

	/// Symbolic names for week day numbers (0 to 6).
	enum DaysOfWeek
	{
		SUNDAY = 0,
		MONDAY,
		TUESDAY,
		WEDNESDAY,
		THURSDAY,
		FRIDAY,
		SATURDAY
	};

	DateTime();
	DateTime(const Timestamp& timestamp);
	DateTime(int year, int month, int day, int hour = 0, int minute = 0, int second = 0, int millisecond = 0, int microsecond = 0);
	DateTime(double julianDay);
	DateTime(int64_t utcTime, int64_t diff);
	DateTime(const DateTime& dateTime);
	~DateTime();

	DateTime& operator = (const DateTime& dateTime);
	DateTime& operator = (const Timestamp& timestamp);
	DateTime& operator = (double julianDay);

	DateTime& assign(int year, int month, int day, int hour = 0, int minute = 0, int second = 0, int millisecond = 0, int microseconds = 0);

	void swap(DateTime& dateTime);

	int year() const;
	int month() const;
	/// Returns the week number within the year.
	/// FirstDayOfWeek should be either SUNDAY (0) or MONDAY (1).
	/// The returned week number will be from 0 to 53. Week number 1 is the week 
	/// containing January 4. This is in accordance to ISO 8601.
	/// 
	/// The following example assumes that firstDayOfWeek is MONDAY. For 2005, which started
	/// on a Saturday, week 1 will be the week starting on Monday, January 3.
	/// January 1 and 2 will fall within week 0 (or the last week of the previous year).
	///
	/// For 2007, which starts on a Monday, week 1 will be the week starting on Monday, January 1.
	/// There will be no week 0 in 2007.
	int week(int firstDayOfWeek = MONDAY) const;

	/// Returns the day within the month (1 to 31).
	int day() const;

	/// Returns the weekday (0 to 6, where
	/// 0 = Sunday, 1 = Monday, ..., 6 = Saturday).
	int dayOfWeek() const;

	/// Returns the number of the day in the year.
	/// January 1 is 1, February 1 is 32, etc.
	int dayOfYear() const;

	/// Returns the hour (0 to 23).
	int hour() const;
	
	/// Returns the hour (0 to 12).
	int hourAMPM() const;

	/// Returns true if hour < 12;
	bool isAM() const;

	/// Returns true if hour >= 12.
	bool isPM() const;

	/// Returns the minute (0 to 59).
	int minute() const;

	/// Returns the second (0 to 59).
	int second() const;

	/// Returns the millisecond (0 to 999)
	int millisecond() const;

	/// Returns the microsecond (0 to 999)
	int microsecond() const;

	/// Returns the julian day for the date and time.
	double julianDay() const;

	/// Returns the date and time expressed as a Timestamp.
	Timestamp timestamp() const;

	/// Returns the date and time expressed in UTC-based
	/// time. UTC base time is midnight, October 15, 1582.
	/// Resolution is 100 nanoseconds.
	int64_t utcTime() const;

	bool operator == (const DateTime& dateTime) const;
	bool operator != (const DateTime& dateTime) const;
	bool operator <  (const DateTime& dateTime) const;
	bool operator <= (const DateTime& dateTime) const;
	bool operator >  (const DateTime& dateTime) const;
	bool operator >= (const DateTime& dateTime) const;

	Timespan  operator -  (const DateTime& other) const;
	DateTime  operator +  (const Timespan& span) const;
	DateTime  operator -  (const Timespan& span) const;
	DateTime& operator += (const Timespan& span);
	DateTime& operator -= (const Timespan& span);

	/// Converts a local time into UTC, by applying the given time zone differential.
	void makeUTC(int tzd);
	
	/// Converts a UTC time into a local time, by applying the given time zone differential.
	void makeLocal(int tzd);

	/// Returns true if the given year is a leap year; 
	/// false otherwise.
	static bool isLeapYear(int year);

	/// Returns the number of days in the given month
	/// and year. Month is from 1 to 12.
	static int daysOfMonth(int year, int month);

	/// Checks if the given date and time is valid
	/// (all arguments are within a proper range).
	///
	/// Returns true if all arguments are valid, false otherwise.
	static bool isValid(int year, int month, int day, int hour = 0, int minute = 0, int second = 0, int millisecond = 0, int microsecond = 0);

protected:
	/// Computes the Julian day for an UTC time.
	static double toJulianDay(int64_t utcTime);

	/// Computes the Julian day for a Gregorian calendar date and time.
	/// See <http://vsg.cape.com/~pbaum/date/jdimp.htm>, section 2.3.1 for the algorithm.
	static double toJulianDay(int year, int month, int day, int hour = 0, int minute = 0, int second = 0, int millisecond = 0, int microsecond = 0);

	/// Computes the UTC time for a Julian day.
	static int64_t toUtcTime(double julianDay);

	/// Computes the Gregorian date for the given Julian day.
	/// See <http://vsg.cape.com/~pbaum/date/injdimp.htm>, section 3.3.1 for the algorithm.
	void computeGregorian(double julianDay);

	/// Extracts the daytime (hours, minutes, seconds, etc.) from the stored utcTime.
	void computeDaytime();

private:
	void checkLimit(short& lower, short& higher, short limit);
	void normalize();
	///utility functions used to correct the overflow in computeGregorian

	int64 m_utcTime;
	short m_year;
	short m_month;
	short m_day;
	short m_hour;
	short m_minute;
	short m_second;
	short m_millisecond;
	short m_microsecond;
};

//
// inlines
//
inline Timestamp DateTime::timestamp() const
{
	return Timestamp::fromUtcTime(m_utcTime);
}

inline int64_t DateTime::utcTime() const
{
	return m_utcTime;
}

inline int DateTime::year() const
{
	return m_year;
}

inline int DateTime::month() const
{
	return m_month;
}

inline int DateTime::day() const
{
	return m_day;
}

inline int DateTime::hour() const
{
	return m_hour;
}

inline int DateTime::hourAMPM() const
{
	if (m_hour < 1)
		return 12;
	else if (m_hour > 12)
		return m_hour - 12;
	else
		return m_hour;
}

inline bool DateTime::isAM() const
{
	return m_hour < 12;
}

inline bool DateTime::isPM() const
{
	return m_hour >= 12;
}

inline int DateTime::minute() const
{
	return m_minute;
}

inline int DateTime::second() const
{
	return m_second;
}

inline int DateTime::millisecond() const
{
	return m_millisecond;
}

inline int DateTime::microsecond() const
{
	return m_microsecond;
}

inline bool DateTime::operator == (const DateTime& dateTime) const
{
	return m_utcTime == dateTime.m_utcTime;
}

inline bool DateTime::operator != (const DateTime& dateTime) const
{
	return m_utcTime != dateTime.m_utcTime;
}

inline bool DateTime::operator < (const DateTime& dateTime) const
{
	return m_utcTime < dateTime.m_utcTime;
}

inline bool DateTime::operator <= (const DateTime& dateTime) const
{
	return m_utcTime <= dateTime.m_utcTime;
}

inline bool DateTime::operator >(const DateTime& dateTime) const
{
	return m_utcTime > dateTime.m_utcTime;
}

inline bool DateTime::operator >= (const DateTime& dateTime) const
{
	return m_utcTime >= dateTime.m_utcTime;
}

inline bool DateTime::isLeapYear(int year)
{
	return (year % 4) == 0 && ((year % 100) != 0 || (year % 400) == 0);
}

inline void swap(DateTime& d1, DateTime& d2)
{
	d1.swap(d2);
}

CUTE_NS_END
