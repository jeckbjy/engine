// module DateTime
#include "Cute/DateTime.h"

CUTE_NS_BEGIN

inline double DateTime::toJulianDay(int64_t utcTime)
{
	double utcDays = double(utcTime) / 864000000000.0;
	return utcDays + 2299160.5; // first day of Gregorian reform (Oct 15 1582)
}

inline int64_t DateTime::toUtcTime(double julianDay)
{
	return int64_t((julianDay - 2299160.5)*864000000000.0);
}

DateTime::DateTime()
{
	Timestamp now;
	m_utcTime = now.utcTime();
	computeGregorian(julianDay());
	computeDaytime();
}

DateTime::DateTime(const Timestamp& rTimestamp) 
	: m_utcTime(rTimestamp.utcTime())
{
	computeGregorian(julianDay());
	computeDaytime();
}

DateTime::DateTime(int otherYear, int otherMonth, int otherDay, int otherHour, int otherMinute, int otherSecond, int otherMillisecond, int otherMicrosecond) 
	: m_year(otherYear)
	, m_month(otherMonth)
	, m_day(otherDay)
	, m_hour(otherHour)
	, m_minute(otherMinute)
	, m_second(otherSecond)
	, m_millisecond(otherMillisecond)
	, m_microsecond(otherMicrosecond)
{
	cute_assert(m_year >= 0 && m_year <= 9999);
	cute_assert(m_month >= 1 && m_month <= 12);
	cute_assert(m_day >= 1 && m_day <= daysOfMonth(m_year, m_month));
	cute_assert(m_hour >= 0 && m_hour <= 23);
	cute_assert(m_minute >= 0 && m_minute <= 59);
	cute_assert(m_second >= 0 && m_second <= 59);
	cute_assert(m_millisecond >= 0 && m_millisecond <= 999);
	cute_assert(m_microsecond >= 0 && m_microsecond <= 999);

	m_utcTime = toUtcTime(toJulianDay(m_year, m_month, m_day)) + 10 * (m_hour*Timespan::HOURS + m_minute*Timespan::MINUTES + m_second*Timespan::SECONDS + m_millisecond*Timespan::MILLISECONDS + m_microsecond);
}

DateTime::DateTime(double otherJulianDay) 
	: m_utcTime(toUtcTime(otherJulianDay))
{
	computeGregorian(otherJulianDay);
}

DateTime::DateTime(int64_t otherUtcTime, int64_t diff) 
	: m_utcTime(otherUtcTime + diff * 10)
{
	computeGregorian(julianDay());
	computeDaytime();
}

DateTime::DateTime(const DateTime& dateTime) 
	: m_utcTime(dateTime.m_utcTime)
	, m_year(dateTime.m_year)
	, m_month(dateTime.m_month)
	, m_day(dateTime.m_day)
	, m_hour(dateTime.m_hour)
	, m_minute(dateTime.m_minute)
	, m_second(dateTime.m_second)
	, m_millisecond(dateTime.m_millisecond)
	, m_microsecond(dateTime.m_microsecond)
{
}

DateTime::~DateTime()
{
}

DateTime& DateTime::operator = (const DateTime& dateTime)
{
	if (&dateTime != this)
	{
		m_utcTime = dateTime.m_utcTime;
		m_year = dateTime.m_year;
		m_month = dateTime.m_month;
		m_day = dateTime.m_day;
		m_hour = dateTime.m_hour;
		m_minute = dateTime.m_minute;
		m_second = dateTime.m_second;
		m_millisecond = dateTime.m_millisecond;
		m_microsecond = dateTime.m_microsecond;
	}
	return *this;
}

DateTime& DateTime::operator = (const Timestamp& otherTimestamp)
{
	m_utcTime = otherTimestamp.utcTime();
	computeGregorian(julianDay());
	computeDaytime();
	return *this;
}

DateTime& DateTime::operator = (double otherJulianDay)
{
	m_utcTime = toUtcTime(otherJulianDay);
	computeGregorian(otherJulianDay);
	return *this;
}

DateTime& DateTime::assign(int otherYear, int otherMonth, int otherDay, int otherHour, int otherMinute, int otherSecond, int otherMillisecond, int otherMicrosecond)
{
	cute_assert(otherYear >= 0 && otherYear <= 9999);
	cute_assert(otherMonth >= 1 && otherMonth <= 12);
	cute_assert(otherDay >= 1 && otherDay <= daysOfMonth(otherYear, otherMonth));
	cute_assert(otherHour >= 0 && otherHour <= 23);
	cute_assert(otherMinute >= 0 && otherMinute <= 59);
	cute_assert(otherSecond >= 0 && otherSecond <= 59);
	cute_assert(otherMillisecond >= 0 && otherMillisecond <= 999);
	cute_assert(otherMicrosecond >= 0 && otherMicrosecond <= 999);

	m_utcTime = toUtcTime(toJulianDay(otherYear, otherMonth, otherDay)) + 10 * (otherHour*Timespan::HOURS + otherMinute*Timespan::MINUTES + otherSecond*Timespan::SECONDS + otherMillisecond*Timespan::MILLISECONDS + otherMicrosecond);
	m_year = otherYear;
	m_month = otherMonth;
	m_day = otherDay;
	m_hour = otherHour;
	m_minute = otherMinute;
	m_second = otherSecond;
	m_millisecond = otherMillisecond;
	m_microsecond = otherMicrosecond;

	return *this;
}

void DateTime::swap(DateTime& dateTime)
{
	std::swap(m_utcTime, dateTime.m_utcTime);
	std::swap(m_year, dateTime.m_year);
	std::swap(m_month, dateTime.m_month);
	std::swap(m_day, dateTime.m_day);
	std::swap(m_hour, dateTime.m_hour);
	std::swap(m_minute, dateTime.m_minute);
	std::swap(m_second, dateTime.m_second);
	std::swap(m_millisecond, dateTime.m_millisecond);
	std::swap(m_microsecond, dateTime.m_microsecond);
}

int DateTime::dayOfWeek() const
{
	return int((std::floor(julianDay() + 1.5))) % 7;
}

int DateTime::dayOfYear() const
{
	int doy = 0;
	for (int currentMonth = 1; currentMonth < m_month; ++currentMonth)
		doy += daysOfMonth(m_year, currentMonth);
	doy += m_day;
	return doy;
}

int DateTime::daysOfMonth(int year, int month)
{
	cute_assert(month >= 1 && month <= 12);

	static int daysOfMonthTable[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	if (month == 2 && isLeapYear(year))
		return 29;
	else
		return daysOfMonthTable[month];
}

bool DateTime::isValid(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond)
{
	return
		(year >= 0 && year <= 9999) &&
		(month >= 1 && month <= 12) &&
		(day >= 1 && day <= daysOfMonth(year, month)) &&
		(hour >= 0 && hour <= 23) &&
		(minute >= 0 && minute <= 59) &&
		(second >= 0 && second <= 59) &&
		(millisecond >= 0 && millisecond <= 999) &&
		(microsecond >= 0 && microsecond <= 999);
}

int DateTime::week(int firstDayOfWeek) const
{
	cute_assert(firstDayOfWeek >= 0 && firstDayOfWeek <= 6);

	/// find the first firstDayOfWeek.
	int baseDay = 1;
	while (DateTime(m_year, 1, baseDay).dayOfWeek() != firstDayOfWeek) ++baseDay;

	int doy = dayOfYear();
	int offs = baseDay <= 4 ? 0 : 1;
	if (doy < baseDay)
		return offs;
	else
		return (doy - baseDay) / 7 + 1 + offs;
}

double DateTime::julianDay() const
{
	return toJulianDay(m_utcTime);
}

DateTime DateTime::operator + (const Timespan& span) const
{
	return DateTime(m_utcTime, span.totalMicroseconds());
}

DateTime DateTime::operator - (const Timespan& span) const
{
	return DateTime(m_utcTime, -span.totalMicroseconds());
}

Timespan DateTime::operator - (const DateTime& dateTime) const
{
	return Timespan((m_utcTime - dateTime.m_utcTime) / 10);
}

DateTime& DateTime::operator += (const Timespan& span)
{
	m_utcTime += span.totalMicroseconds() * 10;
	computeGregorian(julianDay());
	computeDaytime();
	return *this;
}

DateTime& DateTime::operator -= (const Timespan& span)
{
	m_utcTime -= span.totalMicroseconds() * 10;
	computeGregorian(julianDay());
	computeDaytime();
	return *this;
}

void DateTime::makeUTC(int tzd)
{
	operator -= (Timespan(((int64_t) tzd)*Timespan::SECONDS));
}

void DateTime::makeLocal(int tzd)
{
	operator += (Timespan(((int64_t) tzd)*Timespan::SECONDS));
}

double DateTime::toJulianDay(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond)
{
	// lookup table for (153*month - 457)/5 - note that 3 <= month <= 14.
	static int lookup[] = { -91, -60, -30, 0, 31, 61, 92, 122, 153, 184, 214, 245, 275, 306, 337 };

	// day to double
	double dday = double(day) + ((double((hour * 60 + minute) * 60 + second) * 1000 + millisecond) * 1000 + microsecond) / 86400000000.0;
	if (month < 3)
	{
		month += 12;
		--year;
	}
	double dyear = double(year);
	return dday + lookup[month] + 365 * year + std::floor(dyear / 4) - std::floor(dyear / 100) + std::floor(dyear / 400) + 1721118.5;
}

void DateTime::checkLimit(short& lower, short& higher, short limit)
{
	if (lower >= limit)
	{
		higher += short(lower / limit);
		lower = short(lower % limit);
	}
}

void DateTime::normalize()
{
	checkLimit(m_microsecond, m_millisecond, 1000);
	checkLimit(m_millisecond, m_second, 1000);
	checkLimit(m_second, m_minute, 60);
	checkLimit(m_minute, m_hour, 60);
	checkLimit(m_hour, m_day, 24);

	if (m_day > daysOfMonth(m_year, m_month))
	{
		m_day -= daysOfMonth(m_year, m_month);
		if (++m_month > 12)
		{
			++m_year;
			m_month -= 12;
		}
	}
}

void DateTime::computeGregorian(double otherJulianDay)
{
	double z = std::floor(otherJulianDay - 1721118.5);
	double r = otherJulianDay - 1721118.5 - z;
	double g = z - 0.25;
	double a = std::floor(g / 36524.25);
	double b = a - std::floor(a / 4);
	m_year = short(std::floor((b + g) / 365.25));
	double c = b + z - std::floor(365.25*m_year);
	m_month = short(std::floor((5 * c + 456) / 153));
	double dday = c - std::floor((153.0*m_month - 457) / 5) + r;
	m_day = short(dday);
	if (m_month > 12)
	{
		++m_year;
		m_month -= 12;
	}
	r *= 24;
	m_hour = short(std::floor(r));
	r -= std::floor(r);
	r *= 60;
	m_minute = short(std::floor(r));
	r -= std::floor(r);
	r *= 60;
	m_second = short(std::floor(r));
	r -= std::floor(r);
	r *= 1000;
	m_millisecond = short(std::floor(r));
	r -= std::floor(r);
	r *= 1000;
	m_microsecond = short(r + 0.5);

	normalize();

	cute_assert_dbg(m_month >= 1 && m_month <= 12);
	cute_assert_dbg(m_day >= 1 && m_day <= daysOfMonth(m_year, m_month));
	cute_assert_dbg(m_hour >= 0 && m_hour <= 23);
	cute_assert_dbg(m_minute >= 0 && m_minute <= 59);
	cute_assert_dbg(m_second >= 0 && m_second <= 59);
	cute_assert_dbg(m_millisecond >= 0 && m_millisecond <= 999);
	cute_assert_dbg(m_microsecond >= 0 && m_microsecond <= 999);
}

void DateTime::computeDaytime()
{
	Timespan span(m_utcTime / 10);
	int spanHour = span.hours();
	// Due to double rounding issues, the previous call to computeGregorian()
	// may have crossed into the next or previous day. We need to correct that.
	if (spanHour == 23 && m_hour == 0)
	{
		m_day--;
		if (m_day == 0)
		{
			m_month--;
			if (m_month == 0)
			{
				m_month = 12;
				m_year--;
			}
			m_day = daysOfMonth(m_year, m_month);
		}
	}
	else if (spanHour == 0 && m_hour == 23)
	{
		m_day++;
		if (m_day > daysOfMonth(m_year, m_month))
		{
			m_month++;
			if (m_month > 12)
			{
				m_month = 1;
				m_year++;
			}
			m_day = 1;
		}
	}
	m_hour = spanHour;
	m_minute = span.minutes();
	m_second = span.seconds();
	m_millisecond = span.milliseconds();
	m_microsecond = span.microseconds();
}

CUTE_NS_END