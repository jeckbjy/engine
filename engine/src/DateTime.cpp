// module DateTime
#include "Cute/DateTime.h"
#include "Cute/Number.h"
#include "Cute/Ascii.h"
#include "Cute/Exception.h"

CUTE_NS_BEGIN

const String DateTime::ISO8601_FORMAT("%Y-%m-%dT%H:%M:%S%z");
const String DateTime::ISO8601_FRAC_FORMAT("%Y-%m-%dT%H:%M:%s%z");
const String DateTime::RFC822_FORMAT("%w, %e %b %y %H:%M:%S %Z");
const String DateTime::RFC1123_FORMAT("%w, %e %b %Y %H:%M:%S %Z");
const String DateTime::HTTP_FORMAT("%w, %d %b %Y %H:%M:%S %Z");
const String DateTime::RFC850_FORMAT("%W, %e-%b-%y %H:%M:%S %Z");
const String DateTime::RFC1036_FORMAT("%W, %e %b %y %H:%M:%S %Z");
const String DateTime::ASCTIME_FORMAT("%w %b %f %H:%M:%S %Y");
const String DateTime::SORTABLE_FORMAT("%Y-%m-%d %H:%M:%S");
const String DateTime::SPECIAL_FORMAT("%w, %e %b %r %H:%M:%S %Z");

const String DateTime::WEEKDAY_NAMES[] =
{
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
};


const String DateTime::MONTH_NAMES[] =
{
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
};

//////////////////////////////////////////////////////////////////////////
// static
//////////////////////////////////////////////////////////////////////////
int DateTime::daysOfMonth(int year, int month)
{
    assert(month >= 1 && month <= 12);

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

bool DateTime::parseDayOfWeek(int& day, const String& str)
{
    String::const_iterator it  = str.begin();
    String::const_iterator end = str.end();
    
    String dow;
    while (it != end && (Ascii::isSpace(*it) || Ascii::isPunct(*it)))
        ++it;

    bool isFirst = true;
    while (it != end && Ascii::isAlpha(*it))
    {
        char ch = (*it++);
        if (isFirst) { dow += Ascii::toUpper(ch); isFirst = false; }
        else dow += Ascii::toLower(ch);
    }

    if (dow.length() < 3)
        return false;
        // throw SyntaxException("Weekday name must be at least three characters long", dow);
    
    for (int i = 0; i < 7; ++i)
    {
        if (WEEKDAY_NAMES[i].find(dow) == 0){
            day = i;
            return true;
        }
    }
    return false;
    // throw SyntaxException("Not a valid weekday name", dow);
}

//////////////////////////////////////////////////////////////////////////
// parse
//////////////////////////////////////////////////////////////////////////
#define SKIP_JUNK() \
    while (it != end && !Ascii::isDigit(*it)) ++it

#define SKIP_DIGITS() \
    while (it != end && Ascii::isDigit(*it)) ++it

#define PARSE_NUMBER(var) \
    while (it != end && Ascii::isDigit(*it)) var = var*10 + ((*it++) - '0')

#define PARSE_NUMBER_N(var, n) \
    { int i = 0; while (i++ < n && it != end && Ascii::isDigit(*it)) var = var*10 + ((*it++) - '0'); }

#define PARSE_FRACTIONAL_N(var, n) \
    { int i = 0; while (i < n && it != end && Ascii::isDigit(*it)) { var = var*10 + ((*it++) - '0'); i++; } while (i++ < n) var *= 10; }

static int parseTZD(String::const_iterator& it, const String::const_iterator& end)
{
    struct Zone
    {
        const char* designator;
        int         tzd;
    };

    static Zone zones[] =
    {
        {"Z",           0},
        {"UT",          0},
        {"GMT",         0},
        {"BST",    1 * 3600},
        {"IST",    1 * 3600},
        {"WET",         0},
        {"WEST",   1 * 3600},
        {"CET",    1 * 3600},
        {"CEST",   2 * 3600},
        {"EET",    2 * 3600},
        {"EEST",   3 * 3600},
        {"MSK",    3 * 3600},
        {"MSD",    4 * 3600},
        {"NST",   -3 * 3600 - 1800},
        {"NDT",   -2 * 3600 - 1800},
        {"AST",   -4 * 3600},
        {"ADT",   -3 * 3600},
        {"EST",   -5 * 3600},
        {"EDT",   -4 * 3600},
        {"CST",   -6 * 3600},
        {"CDT",   -5 * 3600},
        {"MST",   -7 * 3600},
        {"MDT",   -6 * 3600},
        {"PST",   -8 * 3600},
        {"PDT",   -7 * 3600},
        {"AKST",  -9 * 3600},
        {"AKDT",  -8 * 3600},
        {"HST",  -10 * 3600},
        {"AEST",  10 * 3600},
        {"AEDT",  11 * 3600},
        {"ACST",   9 * 3600 + 1800},
        {"ACDT",  10 * 3600 + 1800},
        {"AWST",   8 * 3600},
        {"AWDT",   9 * 3600}
    };

    int tzd = 0;
    while (it != end && Ascii::isSpace(*it)) ++it;
    if (it != end)
    {
        if (Ascii::isAlpha(*it))
        {
            String designator;
            designator += *it++;
            if (it != end && Ascii::isAlpha(*it)) designator += *it++;
            if (it != end && Ascii::isAlpha(*it)) designator += *it++;
            if (it != end && Ascii::isAlpha(*it)) designator += *it++;
            for (unsigned i = 0; i < sizeof(zones) / sizeof(Zone); ++i)
            {
                if (designator == zones[i].designator)
                {
                    tzd = zones[i].tzd;
                    break;
                }
            }
        }
        if (it != end && (*it == '+' || *it == '-'))
        {
            int sign = *it == '+' ? 1 : -1;
            ++it;
            int hours = 0;
            PARSE_NUMBER_N(hours, 2);
            if (it != end && *it == ':') ++it;
            int minutes = 0;
            PARSE_NUMBER_N(minutes, 2);
            tzd += sign * (hours * 3600 + minutes * 60);
        }
    }

    return tzd;
}


static int parseMonth(String::const_iterator& it, const String::const_iterator& end)
{
    String month;

    while (it != end && (Ascii::isSpace(*it) || Ascii::isPunct(*it)))
        ++it;

    bool isFirst = true;
    while (it != end && Ascii::isAlpha(*it))
    {
        char ch = (*it++);
        if (isFirst) { month += Ascii::toUpper(ch); isFirst = false; }
        else month += Ascii::toLower(ch);
    }

    if (month.length() < 3)
        throw SyntaxException("Month name must be at least three characters long", month);

    for (int i = 0; i < 12; ++i)
    {
        if (DateTime::MONTH_NAMES[i].find(month) == 0)
            return i + 1;
    }

    throw SyntaxException("Not a valid month name", month);
}

static int parseAMPM(String::const_iterator& it, const String::const_iterator& end, int hour)
{
    String ampm;
    while (it != end && (Ascii::isSpace(*it) || Ascii::isPunct(*it)))
        ++it;

    while (it != end && Ascii::isAlpha(*it))
    {
        char ch = (*it++);
        ampm += Ascii::toUpper(ch);
    }

    if (ampm == "AM")
    {
        if (hour == 12)
            return 0;
        else
            return hour;
    }
    else if (ampm == "PM")
    {
        if (hour < 12)
            return hour + 12;
        else
            return hour;
    }
    else throw SyntaxException("Not a valid AM/PM designator", ampm);
}

bool DateTime::parse(DateTime& datetime, const String& str, const String& fmt)
{
    int year   = 0;
    int month  = 0;
    int day    = 0;
    int hour   = 0;
    int minute = 0;
    int second = 0;
    int millis = 0;
    int micros = 0;
    int tzd    = 0;

    String::const_iterator it   = str.begin();
    String::const_iterator end  = str.end();
    String::const_iterator itf  = fmt.begin();
    String::const_iterator endf = fmt.end();

    while (itf != endf && it != end)
    {
        if (*itf == '%')
        {
            if (++itf != endf)
            {
                switch (*itf)
                {
                case 'a':
                case 'A':
                    hour = parseAMPM(it, end, hour);
                    break;
                case 'w':
                case 'W':
                    while (it != end && Ascii::isSpace(*it)) ++it;
                    while (it != end && Ascii::isAlpha(*it)) ++it;
                    break;
                case 'b':
                case 'B':
                    month = parseMonth(it, end);
                    break;
                case 'd':
                case 'e':
                case 'f':
                    SKIP_JUNK();
                    PARSE_NUMBER_N(day, 2);
                    break;
                case 'm':
                case 'n':
                case 'o':
                    SKIP_JUNK();
                    PARSE_NUMBER_N(month, 2);
                    break;
                case 'y':
                    SKIP_JUNK();
                    PARSE_NUMBER_N(year, 2);
                    if (year >= 69)
                        year += 1900;
                    else
                        year += 2000;
                    break;
                case 'Y':
                    SKIP_JUNK();
                    PARSE_NUMBER_N(year, 4);
                    break;
                case 'r':
                    SKIP_JUNK();
                    PARSE_NUMBER(year);
                    if (year < 1000)
                    {
                        if (year >= 69)
                            year += 1900;
                        else
                            year += 2000;
                    }
                    break;
                case 'H':
                case 'h':
                    SKIP_JUNK();
                    PARSE_NUMBER_N(hour, 2);
                    break;
                case 'M':
                    SKIP_JUNK();
                    PARSE_NUMBER_N(minute, 2);
                    break;
                case 'S':
                    SKIP_JUNK();
                    PARSE_NUMBER_N(second, 2);
                    break;
                case 's':
                    SKIP_JUNK();
                    PARSE_NUMBER_N(second, 2);
                    if (it != end && (*it == '.' || *it == ','))
                    {
                        ++it;
                        PARSE_FRACTIONAL_N(millis, 3);
                        PARSE_FRACTIONAL_N(micros, 3);
                        SKIP_DIGITS();
                    }
                    break;
                case 'i':
                    SKIP_JUNK();
                    PARSE_NUMBER_N(millis, 3);
                    break;
                case 'c':
                    SKIP_JUNK();
                    PARSE_NUMBER_N(millis, 1);
                    millis *= 100;
                    break;
                case 'F':
                    SKIP_JUNK();
                    PARSE_FRACTIONAL_N(millis, 3);
                    PARSE_FRACTIONAL_N(micros, 3);
                    SKIP_DIGITS();
                    break;
                case 'z':
                case 'Z':
                    tzd = parseTZD(it, end);
                    break;
                }
                ++itf;
            }
        }
        else ++itf;
    }

    if (month == 0) month = 1;
    if (day == 0) day = 1;

    if (DateTime::isValid(year, month, day, hour, minute, second, millis, micros))
    {
        datetime.assign(year, month, day, hour, minute, second, millis, micros);
        // timeZoneDifferential = tzd;
        return true;
    }
    else
    {
        return false;
    }
}

bool DateTime::parse(Cute::DateTime &dt, const String &str)
{
    if (str.length() < 4)
        return false;

    if (str[3] == ',')
        return parse(dt, str, SPECIAL_FORMAT);
    else if (str[3] == ' ')
        return parse(dt, str, ASCTIME_FORMAT);
    else if (str.find(',') < 10)
        return parse(dt, str, SPECIAL_FORMAT);
    else if (Ascii::isDigit(str[0]))
    {
        if (str.find(' ') != String::npos || str.length() == 10)
            return parse(dt, str, SORTABLE_FORMAT);
        else if (str.find('.') != String::npos || str.find(',') != String::npos)
            return parse(dt, str, ISO8601_FRAC_FORMAT);
        else
            return parse(dt, str, ISO8601_FORMAT);
    }
    else
        return false;
}

//////////////////////////////////////////////////////////////////////////
// format
//////////////////////////////////////////////////////////////////////////
void DateTime::tzdISO(String& str, int timeZoneDifferential)
{
    if (timeZoneDifferential != UTC)
    {
        if (timeZoneDifferential >= 0)
        {
            str += '+';
            str.append0(timeZoneDifferential / 3600, 2);
            str += ':';
            str.append0((timeZoneDifferential % 3600) / 60, 2);
        }
        else
        {
            str += '-';
            str.append0(-timeZoneDifferential / 3600, 2);
            str += ':';
            str.append0((-timeZoneDifferential % 3600) / 60, 2);
        }
    }
    else str += 'Z';
}

void DateTime::tzdRFC(String& str, int timeZoneDifferential)
{
    if (timeZoneDifferential != UTC)
    {
        if (timeZoneDifferential >= 0)
        {
            str += '+';
            str.append0(timeZoneDifferential / 3600, 2);
            str.append0((timeZoneDifferential % 3600) / 60, 2);
        }
        else
        {
            str += '-';
            str.append0(-timeZoneDifferential / 3600, 2);
            str.append0((-timeZoneDifferential % 3600) / 60, 2);
        }
    }
    else str += "GMT";
}

void DateTime::format(String& str, const DateTime& dt, const String& fmt)
{
    int timeZoneDifferential = UTC_ZONE_DIFF;

    String::const_iterator it  = fmt.begin();
    String::const_iterator end = fmt.end();
    String::value_type ch;
    while (it != end)
    {
        ch = *it++;
        if (ch == '%')
        {
            if (it == end)
                break;

            ch = *it++;

            switch (ch)
            {
            case 'a': str.append(dt.isAM() ? "am" : "pm"); break;
            case 'A': str.append(dt.isAM() ? "AM" : "PM"); break;
            case 'w': str.append(WEEKDAY_NAMES[dt.dayOfWeek()], 0, 3); break;
            case 'W': str.append(WEEKDAY_NAMES[dt.dayOfWeek()]);       break;
            case 'b': str.append(MONTH_NAMES[dt.month() - 1], 0, 3);   break;
            case 'B': str.append(MONTH_NAMES[dt.month() - 1]);         break;
            case 'd': str.append0(dt.day(), 2); break;
            case 'e': str.appends(dt.day()); break;
            case 'f': str.appends(dt.day(), 2); break;
            case 'm': str.append0(dt.month(), 2); break;
            case 'n': str.appends(dt.month()); break;
            case 'o': str.appends(dt.month(), 2); break;
            case 'y': str.append0(dt.year() % 100, 2); break;
            case 'Y': str.append0(dt.year(), 4); break;
            case 'H': str.append0(dt.hour(), 2); break;
            case 'h': str.append0(dt.hourAMPM(), 2); break;
            case 'M': str.append0(dt.minute(), 2); break;
            case 'S': str.append0(dt.second(), 2); break;
            case 's': str.append0(dt.second(), 2);
                str += '.';
                str.append0(dt.millisecond() * 1000 + dt.microsecond(), 6);
                break;
            case 'i': str.append0(dt.millisecond(), 3); break;
            case 'c': str.appends(dt.millisecond() / 100); break;
            case 'F': str.append0(dt.millisecond() * 1000 + dt.microsecond(), 6); break;
            case 'z': tzdISO(str, timeZoneDifferential); break;
            case 'Z': tzdRFC(str, timeZoneDifferential); break;
            default:  str += ch;
            }
        }
        else
        {
            str += ch;
        }
    }
}

//////////////////////////////////////////////////////////////////////////
// DateTime
//////////////////////////////////////////////////////////////////////////
inline void checkLimit(short& lower, short& higher, short limit)
{
    if (lower >= limit)
    {
        higher += short(lower / limit);
        lower = short(lower % limit);
    }
}
inline int64 toUtcTime(double julianDay)
{
    return int64((julianDay - 2299160.5) * 864000000000.0);
}

inline double toJulianDay(int64 utcTime)
{
    double utcDays = double(utcTime) / 864000000000.0;
    return utcDays + 2299160.5; // first day of Gregorian reform (Oct 15 1582)
}

inline double toJulianDay(int year, int month, int day, int hour = 0, int minute = 0, int second = 0, int millisecond = 0, int microsecond = 0)
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

DateTime::DateTime()
{
    Timestamp now;
    m_time = now.utcTime();
    computeGregorian(julianDay());
    computeDaytime();
}

DateTime::DateTime(const Timestamp& ts)
    : m_time(ts.utcTime())
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

    m_time = toUtcTime(toJulianDay(m_year, m_month, m_day)) + 10 * (m_hour * Timespan::HOURS + m_minute * Timespan::MINUTES + m_second * Timespan::SECONDS + m_millisecond * Timespan::MILLISECONDS + m_microsecond);
}

DateTime::DateTime(double otherJulianDay)
    : m_time(toUtcTime(otherJulianDay))
{
    computeGregorian(otherJulianDay);
}

DateTime::DateTime(int64_t otherUtcTime, int64_t diff)
    : m_time(otherUtcTime + diff * 10)
{
    computeGregorian(julianDay());
    computeDaytime();
}

DateTime::DateTime(const DateTime& other)
    : m_time(other.m_time)
    , m_year(other.m_year)
    , m_month(other.m_month)
    , m_day(other.m_day)
    , m_hour(other.m_hour)
    , m_minute(other.m_minute)
    , m_second(other.m_second)
    , m_millisecond(other.m_millisecond)
    , m_microsecond(other.m_microsecond)
{
}

DateTime::~DateTime()
{
}

DateTime& DateTime::operator = (const DateTime& other)
{
    if (&other != this)
    {
        m_time      = other.m_time;
        m_year      = other.m_year;
        m_month     = other.m_month;
        m_day       = other.m_day;
        m_hour      = other.m_hour;
        m_minute    = other.m_minute;
        m_second    = other.m_second;
        m_millisecond = other.m_millisecond;
        m_microsecond = other.m_microsecond;
    }

    return *this;
}

DateTime& DateTime::operator = (const Timestamp& ts)
{
    m_time = ts.utcTime();
    computeGregorian(julianDay());
    computeDaytime();
    return *this;
}

DateTime& DateTime::operator = (double otherJulianDay)
{
    m_time = toUtcTime(otherJulianDay);
    computeGregorian(otherJulianDay);
    return *this;
}

void DateTime::assign(int otherYear, int otherMonth, int otherDay, int otherHour, int otherMinute, int otherSecond, int otherMillisecond, int otherMicrosecond)
{
    cute_assert(otherYear >= 0 && otherYear <= 9999);
    cute_assert(otherMonth >= 1 && otherMonth <= 12);
    cute_assert(otherDay >= 1 && otherDay <= daysOfMonth(otherYear, otherMonth));
    cute_assert(otherHour >= 0 && otherHour <= 23);
    cute_assert(otherMinute >= 0 && otherMinute <= 59);
    cute_assert(otherSecond >= 0 && otherSecond <= 59);
    cute_assert(otherMillisecond >= 0 && otherMillisecond <= 999);
    cute_assert(otherMicrosecond >= 0 && otherMicrosecond <= 999);

    m_time = toUtcTime(toJulianDay(otherYear, otherMonth, otherDay)) + 10 * (otherHour * Timespan::HOURS + otherMinute * Timespan::MINUTES + otherSecond * Timespan::SECONDS + otherMillisecond * Timespan::MILLISECONDS + otherMicrosecond);
    m_year = otherYear;
    m_month = otherMonth;
    m_day = otherDay;
    m_hour = otherHour;
    m_minute = otherMinute;
    m_second = otherSecond;
    m_millisecond = otherMillisecond;
    m_microsecond = otherMicrosecond;
}

void DateTime::swap(DateTime& other)
{
    std::swap(m_time,   other.m_time);
    std::swap(m_year,   other.m_year);
    std::swap(m_month,  other.m_month);
    std::swap(m_day,    other.m_day);
    std::swap(m_hour,   other.m_hour);
    std::swap(m_minute, other.m_minute);
    std::swap(m_second, other.m_second);
    std::swap(m_millisecond, other.m_millisecond);
    std::swap(m_microsecond, other.m_microsecond);
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
    return toJulianDay(m_time);
}

DateTime DateTime::operator + (const Timespan& span) const
{
    return DateTime(m_time, span.totalMicroseconds());
}

DateTime DateTime::operator - (const Timespan& span) const
{
    return DateTime(m_time, -span.totalMicroseconds());
}

Timespan DateTime::operator - (const DateTime& other) const
{
    return Timespan((m_time - other.m_time) / 10);
}

DateTime& DateTime::operator += (const Timespan& span)
{
    m_time += span.totalMicroseconds() * 10;
    computeGregorian(julianDay());
    computeDaytime();
    return *this;
}

DateTime& DateTime::operator -= (const Timespan& span)
{
    m_time -= span.totalMicroseconds() * 10;
    computeGregorian(julianDay());
    computeDaytime();
    return *this;
}

// void DateTime::makeUTC(int tzd)
// {
//     operator -= (Timespan(((int64_t) tzd)*Timespan::SECONDS));
// }

// void DateTime::makeLocal(int tzd)
// {
//     operator += (Timespan(((int64_t) tzd)*Timespan::SECONDS));
// }


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
    double c = b + z - std::floor(365.25 * m_year);
    m_month = short(std::floor((5 * c + 456) / 153));
    double dday = c - std::floor((153.0 * m_month - 457) / 5) + r;
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
    Timespan span(m_time / 10);
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




//inline double DateTime::toJulianDay(int64_t utcTime)
//{
//  double utcDays = double(utcTime) / 864000000000.0;
//  return utcDays + 2299160.5; // first day of Gregorian reform (Oct 15 1582)
//}
//
//inline int64_t DateTime::toUtcTime(double julianDay)
//{
//  return int64_t((julianDay - 2299160.5)*864000000000.0);
//}
//
//DateTime::DateTime()
//{
//  Timestamp now;
//  m_utcTime = now.utcTime();
//  computeGregorian(julianDay());
//  computeDaytime();
//}
//
//DateTime::DateTime(const Timestamp& rTimestamp)
//  : m_utcTime(rTimestamp.utcTime())
//{
//  computeGregorian(julianDay());
//  computeDaytime();
//}
//
//DateTime::DateTime(int otherYear, int otherMonth, int otherDay, int otherHour, int otherMinute, int otherSecond, int otherMillisecond, int otherMicrosecond)
//  : m_year(otherYear)
//  , m_month(otherMonth)
//  , m_day(otherDay)
//  , m_hour(otherHour)
//  , m_minute(otherMinute)
//  , m_second(otherSecond)
//  , m_millisecond(otherMillisecond)
//  , m_microsecond(otherMicrosecond)
//{
//  cute_assert(m_year >= 0 && m_year <= 9999);
//  cute_assert(m_month >= 1 && m_month <= 12);
//  cute_assert(m_day >= 1 && m_day <= daysOfMonth(m_year, m_month));
//  cute_assert(m_hour >= 0 && m_hour <= 23);
//  cute_assert(m_minute >= 0 && m_minute <= 59);
//  cute_assert(m_second >= 0 && m_second <= 59);
//  cute_assert(m_millisecond >= 0 && m_millisecond <= 999);
//  cute_assert(m_microsecond >= 0 && m_microsecond <= 999);
//
//  m_utcTime = toUtcTime(toJulianDay(m_year, m_month, m_day)) + 10 * (m_hour*Timespan::HOURS + m_minute*Timespan::MINUTES + m_second*Timespan::SECONDS + m_millisecond*Timespan::MILLISECONDS + m_microsecond);
//}
//
//DateTime::DateTime(double otherJulianDay)
//  : m_utcTime(toUtcTime(otherJulianDay))
//{
//  computeGregorian(otherJulianDay);
//}
//
//DateTime::DateTime(int64_t otherUtcTime, int64_t diff)
//  : m_utcTime(otherUtcTime + diff * 10)
//{
//  computeGregorian(julianDay());
//  computeDaytime();
//}
//
//DateTime::DateTime(const DateTime& dateTime)
//  : m_utcTime(dateTime.m_utcTime)
//  , m_year(dateTime.m_year)
//  , m_month(dateTime.m_month)
//  , m_day(dateTime.m_day)
//  , m_hour(dateTime.m_hour)
//  , m_minute(dateTime.m_minute)
//  , m_second(dateTime.m_second)
//  , m_millisecond(dateTime.m_millisecond)
//  , m_microsecond(dateTime.m_microsecond)
//{
//}
//
//DateTime::~DateTime()
//{
//}
//
//DateTime& DateTime::operator = (const DateTime& dateTime)
//{
//  if (&dateTime != this)
//  {
//      m_utcTime = dateTime.m_utcTime;
//      m_year = dateTime.m_year;
//      m_month = dateTime.m_month;
//      m_day = dateTime.m_day;
//      m_hour = dateTime.m_hour;
//      m_minute = dateTime.m_minute;
//      m_second = dateTime.m_second;
//      m_millisecond = dateTime.m_millisecond;
//      m_microsecond = dateTime.m_microsecond;
//  }
//  return *this;
//}
//
//DateTime& DateTime::operator = (const Timestamp& otherTimestamp)
//{
//  m_utcTime = otherTimestamp.utcTime();
//  computeGregorian(julianDay());
//  computeDaytime();
//  return *this;
//}
//
//DateTime& DateTime::operator = (double otherJulianDay)
//{
//  m_utcTime = toUtcTime(otherJulianDay);
//  computeGregorian(otherJulianDay);
//  return *this;
//}
//
//DateTime& DateTime::assign(int otherYear, int otherMonth, int otherDay, int otherHour, int otherMinute, int otherSecond, int otherMillisecond, int otherMicrosecond)
//{
//  cute_assert(otherYear >= 0 && otherYear <= 9999);
//  cute_assert(otherMonth >= 1 && otherMonth <= 12);
//  cute_assert(otherDay >= 1 && otherDay <= daysOfMonth(otherYear, otherMonth));
//  cute_assert(otherHour >= 0 && otherHour <= 23);
//  cute_assert(otherMinute >= 0 && otherMinute <= 59);
//  cute_assert(otherSecond >= 0 && otherSecond <= 59);
//  cute_assert(otherMillisecond >= 0 && otherMillisecond <= 999);
//  cute_assert(otherMicrosecond >= 0 && otherMicrosecond <= 999);
//
//  m_utcTime = toUtcTime(toJulianDay(otherYear, otherMonth, otherDay)) + 10 * (otherHour*Timespan::HOURS + otherMinute*Timespan::MINUTES + otherSecond*Timespan::SECONDS + otherMillisecond*Timespan::MILLISECONDS + otherMicrosecond);
//  m_year = otherYear;
//  m_month = otherMonth;
//  m_day = otherDay;
//  m_hour = otherHour;
//  m_minute = otherMinute;
//  m_second = otherSecond;
//  m_millisecond = otherMillisecond;
//  m_microsecond = otherMicrosecond;
//
//  return *this;
//}
//
//void DateTime::swap(DateTime& dateTime)
//{
//  std::swap(m_utcTime, dateTime.m_utcTime);
//  std::swap(m_year, dateTime.m_year);
//  std::swap(m_month, dateTime.m_month);
//  std::swap(m_day, dateTime.m_day);
//  std::swap(m_hour, dateTime.m_hour);
//  std::swap(m_minute, dateTime.m_minute);
//  std::swap(m_second, dateTime.m_second);
//  std::swap(m_millisecond, dateTime.m_millisecond);
//  std::swap(m_microsecond, dateTime.m_microsecond);
//}
//
//int DateTime::dayOfWeek() const
//{
//  return int((std::floor(julianDay() + 1.5))) % 7;
//}
//
//int DateTime::dayOfYear() const
//{
//  int doy = 0;
//  for (int currentMonth = 1; currentMonth < m_month; ++currentMonth)
//      doy += daysOfMonth(m_year, currentMonth);
//  doy += m_day;
//  return doy;
//}
//
//int DateTime::daysOfMonth(int year, int month)
//{
//  cute_assert(month >= 1 && month <= 12);
//
//  static int daysOfMonthTable[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
//
//  if (month == 2 && isLeapYear(year))
//      return 29;
//  else
//      return daysOfMonthTable[month];
//}
//
//bool DateTime::isValid(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond)
//{
//  return
//      (year >= 0 && year <= 9999) &&
//      (month >= 1 && month <= 12) &&
//      (day >= 1 && day <= daysOfMonth(year, month)) &&
//      (hour >= 0 && hour <= 23) &&
//      (minute >= 0 && minute <= 59) &&
//      (second >= 0 && second <= 59) &&
//      (millisecond >= 0 && millisecond <= 999) &&
//      (microsecond >= 0 && microsecond <= 999);
//}
//
//int DateTime::week(int firstDayOfWeek) const
//{
//  cute_assert(firstDayOfWeek >= 0 && firstDayOfWeek <= 6);
//
//  /// find the first firstDayOfWeek.
//  int baseDay = 1;
//  while (DateTime(m_year, 1, baseDay).dayOfWeek() != firstDayOfWeek) ++baseDay;
//
//  int doy = dayOfYear();
//  int offs = baseDay <= 4 ? 0 : 1;
//  if (doy < baseDay)
//      return offs;
//  else
//      return (doy - baseDay) / 7 + 1 + offs;
//}
//
//double DateTime::julianDay() const
//{
//  return toJulianDay(m_utcTime);
//}
//
//DateTime DateTime::operator + (const Timespan& span) const
//{
//  return DateTime(m_utcTime, span.totalMicroseconds());
//}
//
//DateTime DateTime::operator - (const Timespan& span) const
//{
//  return DateTime(m_utcTime, -span.totalMicroseconds());
//}
//
//Timespan DateTime::operator - (const DateTime& other) const
//{
//  return Timespan((m_utcTime - other.m_utcTime) / 10);
//}
//
//DateTime& DateTime::operator += (const Timespan& span)
//{
//  m_utcTime += span.totalMicroseconds() * 10;
//  computeGregorian(julianDay());
//  computeDaytime();
//  return *this;
//}
//
//DateTime& DateTime::operator -= (const Timespan& span)
//{
//  m_utcTime -= span.totalMicroseconds() * 10;
//  computeGregorian(julianDay());
//  computeDaytime();
//  return *this;
//}
//
//void DateTime::makeUTC(int tzd)
//{
//  operator -= (Timespan(((int64_t) tzd)*Timespan::SECONDS));
//}
//
//void DateTime::makeLocal(int tzd)
//{
//  operator += (Timespan(((int64_t) tzd)*Timespan::SECONDS));
//}
//
//double DateTime::toJulianDay(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond)
//{
//  // lookup table for (153*month - 457)/5 - note that 3 <= month <= 14.
//  static int lookup[] = { -91, -60, -30, 0, 31, 61, 92, 122, 153, 184, 214, 245, 275, 306, 337 };
//
//  // day to double
//  double dday = double(day) + ((double((hour * 60 + minute) * 60 + second) * 1000 + millisecond) * 1000 + microsecond) / 86400000000.0;
//  if (month < 3)
//  {
//      month += 12;
//      --year;
//  }
//  double dyear = double(year);
//  return dday + lookup[month] + 365 * year + std::floor(dyear / 4) - std::floor(dyear / 100) + std::floor(dyear / 400) + 1721118.5;
//}
//
//void DateTime::checkLimit(short& lower, short& higher, short limit)
//{
//  if (lower >= limit)
//  {
//      higher += short(lower / limit);
//      lower = short(lower % limit);
//  }
//}
//
//void DateTime::normalize()
//{
//  checkLimit(m_microsecond, m_millisecond, 1000);
//  checkLimit(m_millisecond, m_second, 1000);
//  checkLimit(m_second, m_minute, 60);
//  checkLimit(m_minute, m_hour, 60);
//  checkLimit(m_hour, m_day, 24);
//
//  if (m_day > daysOfMonth(m_year, m_month))
//  {
//      m_day -= daysOfMonth(m_year, m_month);
//      if (++m_month > 12)
//      {
//          ++m_year;
//          m_month -= 12;
//      }
//  }
//}
//
//void DateTime::computeGregorian(double otherJulianDay)
//{
//  double z = std::floor(otherJulianDay - 1721118.5);
//  double r = otherJulianDay - 1721118.5 - z;
//  double g = z - 0.25;
//  double a = std::floor(g / 36524.25);
//  double b = a - std::floor(a / 4);
//  m_year = short(std::floor((b + g) / 365.25));
//  double c = b + z - std::floor(365.25*m_year);
//  m_month = short(std::floor((5 * c + 456) / 153));
//  double dday = c - std::floor((153.0*m_month - 457) / 5) + r;
//  m_day = short(dday);
//  if (m_month > 12)
//  {
//      ++m_year;
//      m_month -= 12;
//  }
//  r *= 24;
//  m_hour = short(std::floor(r));
//  r -= std::floor(r);
//  r *= 60;
//  m_minute = short(std::floor(r));
//  r -= std::floor(r);
//  r *= 60;
//  m_second = short(std::floor(r));
//  r -= std::floor(r);
//  r *= 1000;
//  m_millisecond = short(std::floor(r));
//  r -= std::floor(r);
//  r *= 1000;
//  m_microsecond = short(r + 0.5);
//
//  normalize();
//
//  cute_assert_dbg(m_month >= 1 && m_month <= 12);
//  cute_assert_dbg(m_day >= 1 && m_day <= daysOfMonth(m_year, m_month));
//  cute_assert_dbg(m_hour >= 0 && m_hour <= 23);
//  cute_assert_dbg(m_minute >= 0 && m_minute <= 59);
//  cute_assert_dbg(m_second >= 0 && m_second <= 59);
//  cute_assert_dbg(m_millisecond >= 0 && m_millisecond <= 999);
//  cute_assert_dbg(m_microsecond >= 0 && m_microsecond <= 999);
//}
//
//void DateTime::computeDaytime()
//{
//  Timespan span(m_utcTime / 10);
//  int spanHour = span.hours();
//  // Due to double rounding issues, the previous call to computeGregorian()
//  // may have crossed into the next or previous day. We need to correct that.
//  if (spanHour == 23 && m_hour == 0)
//  {
//      m_day--;
//      if (m_day == 0)
//      {
//          m_month--;
//          if (m_month == 0)
//          {
//              m_month = 12;
//              m_year--;
//          }
//          m_day = daysOfMonth(m_year, m_month);
//      }
//  }
//  else if (spanHour == 0 && m_hour == 23)
//  {
//      m_day++;
//      if (m_day > daysOfMonth(m_year, m_month))
//      {
//          m_month++;
//          if (m_month > 12)
//          {
//              m_month = 1;
//              m_year++;
//          }
//          m_day = 1;
//      }
//  }
//  m_hour = spanHour;
//  m_minute = span.minutes();
//  m_second = span.seconds();
//  m_millisecond = span.milliseconds();
//  m_microsecond = span.microseconds();
//}

CUTE_NS_END
