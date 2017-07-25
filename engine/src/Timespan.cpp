// module DateTime
#include "Cute/Timespan.h"

CUTE_NS_BEGIN

const int64 Timespan::MILLISECONDS = 1000;
const int64 Timespan::SECONDS 	 = 1000 * Timespan::MILLISECONDS;
const int64 Timespan::MINUTES 	 = 60 * Timespan::SECONDS;
const int64 Timespan::HOURS 		 = 60 * Timespan::MINUTES;
const int64 Timespan::DAYS 		 = 24 * Timespan::HOURS;

String Timespan::format(const Timespan& timespan, const String& fmt)
{
	String str;
	append(str, timespan, fmt);
	return str;
}

void Timespan::append(String& str, const Timespan& timespan, const String& fmt)
{
	String::const_iterator it  = fmt.begin();
	String::const_iterator end = fmt.end();
	String::value_type ch;
	while (it != end)
	{
		ch = *it++;
		if (ch == '%')
		{
			ch = *it++;
			if(it == end)
				break;

			switch (ch)
			{
			case 'd': str.appends(timespan.days()); break;
			case 'H': str.append0(timespan.hours(), 2); break;
			case 'h': str.appends(timespan.totalHours()); break;
			case 'M': str.append0(timespan.minutes(), 2); break;
			case 'm': str.appends(timespan.totalMinutes()); break;
			case 'S': str.append0(timespan.seconds(), 2); break;
			case 's': str.appends(timespan.totalSeconds()); break;
			case 'i': str.append0(timespan.milliseconds(), 3); break;
			case 'c': str.appends(timespan.milliseconds() / 100); break;
			case 'F': str.append0(timespan.milliseconds() * 1000 + timespan.microseconds(), 6); break;
			default:  str += ch;
			}
		}
		else
		{
			str += ch;
		}
	}
}

Timespan::Timespan()
	: m_span(0)
{
}

Timespan::Timespan(int64 microseconds)
	: m_span(microseconds)
{
}

Timespan::Timespan(long seconds, long microseconds)
	: m_span(seconds * SECONDS + microseconds)
{
}

Timespan::Timespan(int days, int hours, int minutes, int seconds, int microseconds)
	: m_span(microseconds + seconds * SECONDS + minutes * MINUTES + hours * HOURS + days * DAYS)
{
}

Timespan::Timespan(const Timespan& timespan)
	: m_span(timespan.m_span)
{
}

Timespan::~Timespan()
{
}

Timespan& Timespan::operator = (const Timespan& timespan)
{
	m_span = timespan.m_span;
	return *this;
}

Timespan& Timespan::operator = (int64 microseconds)
{
	m_span = microseconds;
	return *this;
}

Timespan& Timespan::assign(int days, int hours, int minutes, int seconds, int microseconds)
{
	m_span = microseconds + seconds * SECONDS + minutes * MINUTES + hours * HOURS + days * DAYS;
	return *this;
}

Timespan& Timespan::assign(long seconds, long microseconds)
{
	m_span = seconds * SECONDS + microseconds;
	return *this;
}

void Timespan::swap(Timespan& timespan)
{
	std::swap(m_span, timespan.m_span);
}

Timespan Timespan::operator + (const Timespan& d) const
{
	return Timespan(m_span + d.m_span);
}

Timespan Timespan::operator - (const Timespan& d) const
{
	return Timespan(m_span - d.m_span);
}

Timespan& Timespan::operator += (const Timespan& d)
{
	m_span += d.m_span;
	return *this;
}

Timespan& Timespan::operator -= (const Timespan& d)
{
	m_span -= d.m_span;
	return *this;
}

Timespan Timespan::operator + (int64 microseconds) const
{
	return Timespan(m_span + microseconds);
}

Timespan Timespan::operator - (int64 microseconds) const
{
	return Timespan(m_span - microseconds);
}

Timespan& Timespan::operator += (int64 microseconds)
{
	m_span += microseconds;
	return *this;
}

Timespan& Timespan::operator -= (int64 microseconds)
{
	m_span -= microseconds;
	return *this;
}

CUTE_NS_END
