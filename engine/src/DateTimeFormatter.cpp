//! DateTime
#include "Cute/DateTimeFormatter.h"
#include "Cute/DateTimeFormat.h"
#include "Cute/Timestamp.h"
#include "Cute/Number.h"

CUTE_NS_BEGIN

void DateTimeFormatter::append(String& str, const DateTime& dateTime, const String& fmt, int timeZoneDifferential)
{
	String::const_iterator it = fmt.begin();
	String::const_iterator end = fmt.end();
	while (it != end)
	{
		if (*it == '%')
		{
			if (++it != end)
			{
				switch (*it)
				{
				case 'w': str.append(DateTimeFormat::WEEKDAY_NAMES[dateTime.dayOfWeek()], 0, 3); break;
				case 'W': str.append(DateTimeFormat::WEEKDAY_NAMES[dateTime.dayOfWeek()]); break;
				case 'b': str.append(DateTimeFormat::MONTH_NAMES[dateTime.month() - 1], 0, 3); break;
				case 'B': str.append(DateTimeFormat::MONTH_NAMES[dateTime.month() - 1]); break;
				case 'd': Number::append0(str, dateTime.day(), 2); break;
				case 'e': Number::append(str, dateTime.day()); break;
				case 'f': Number::append(str, dateTime.day(), 2); break;
				case 'm': Number::append0(str, dateTime.month(), 2); break;
				case 'n': Number::append(str, dateTime.month()); break;
				case 'o': Number::append(str, dateTime.month(), 2); break;
				case 'y': Number::append0(str, dateTime.year() % 100, 2); break;
				case 'Y': Number::append0(str, dateTime.year(), 4); break;
				case 'H': Number::append0(str, dateTime.hour(), 2); break;
				case 'h': Number::append0(str, dateTime.hourAMPM(), 2); break;
				case 'a': str.append(dateTime.isAM() ? "am" : "pm"); break;
				case 'A': str.append(dateTime.isAM() ? "AM" : "PM"); break;
				case 'M': Number::append0(str, dateTime.minute(), 2); break;
				case 'S': Number::append0(str, dateTime.second(), 2); break;
				case 's': Number::append0(str, dateTime.second(), 2);
					str += '.';
					Number::append0(str, dateTime.millisecond() * 1000 + dateTime.microsecond(), 6);
					break;
				case 'i': Number::append0(str, dateTime.millisecond(), 3); break;
				case 'c': Number::append(str, dateTime.millisecond() / 100); break;
				case 'F': Number::append0(str, dateTime.millisecond() * 1000 + dateTime.microsecond(), 6); break;
				case 'z': tzdISO(str, timeZoneDifferential); break;
				case 'Z': tzdRFC(str, timeZoneDifferential); break;
				default:  str += *it;
				}
				++it;
			}
		}
		else str += *it++;
	}
}

void DateTimeFormatter::append(String& str, const Timespan& timespan, const String& fmt)
{
	String::const_iterator it = fmt.begin();
	String::const_iterator end = fmt.end();
	while (it != end)
	{
		if (*it == '%')
		{
			if (++it != end)
			{
				switch (*it)
				{
				case 'd': Number::append(str, timespan.days()); break;
				case 'H': Number::append0(str, timespan.hours(), 2); break;
				case 'h': Number::append(str, timespan.totalHours()); break;
				case 'M': Number::append0(str, timespan.minutes(), 2); break;
				case 'm': Number::append(str, timespan.totalMinutes()); break;
				case 'S': Number::append0(str, timespan.seconds(), 2); break;
				case 's': Number::append(str, timespan.totalSeconds()); break;
				case 'i': Number::append0(str, timespan.milliseconds(), 3); break;
				case 'c': Number::append(str, timespan.milliseconds() / 100); break;
				case 'F': Number::append0(str, timespan.milliseconds() * 1000 + timespan.microseconds(), 6); break;
				default:  str += *it;
				}
				++it;
			}
		}
		else str += *it++;
	}
}

void DateTimeFormatter::tzdISO(String& str, int timeZoneDifferential)
{
	if (timeZoneDifferential != UTC)
	{
		if (timeZoneDifferential >= 0)
		{
			str += '+';
			Number::append0(str, timeZoneDifferential / 3600, 2);
			str += ':';
			Number::append0(str, (timeZoneDifferential % 3600) / 60, 2);
		}
		else
		{
			str += '-';
			Number::append0(str, -timeZoneDifferential / 3600, 2);
			str += ':';
			Number::append0(str, (-timeZoneDifferential % 3600) / 60, 2);
		}
	}
	else str += 'Z';
}

void DateTimeFormatter::tzdRFC(String& str, int timeZoneDifferential)
{
	if (timeZoneDifferential != UTC)
	{
		if (timeZoneDifferential >= 0)
		{
			str += '+';
			Number::append0(str, timeZoneDifferential / 3600, 2);
			Number::append0(str, (timeZoneDifferential % 3600) / 60, 2);
		}
		else
		{
			str += '-';
			Number::append0(str, -timeZoneDifferential / 3600, 2);
			Number::append0(str, (-timeZoneDifferential % 3600) / 60, 2);
		}
	}
	else str += "GMT";
}

CUTE_NS_END
