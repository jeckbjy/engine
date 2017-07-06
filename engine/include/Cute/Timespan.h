#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

/// A class that represents time spans up to microsecond resolution.
class CUTE_CORE_API Timespan
{
public:
	/// Formats the given timespan according to the given format.
	/// The format string is used as a template to format the date and
	/// is copied character by character except for the following special characters,
	/// which are replaced by the corresponding value.
	///
	///   * %d - days
	///   * %H - hours	 (00 .. 23)
	///   * %h - total hours (0 .. n)
	///   * %M - minutes (00 .. 59)
	///   * %m - total minutes (0 .. n)
	///   * %S - seconds (00 .. 59)
	///   * %s - total seconds (0 .. n)
	///   * %i - milliseconds (000 .. 999)
	///   * %c - centisecond (0 .. 9)
	///   * %F - fractional seconds/microseconds (000000 - 999999)
	///   * %% - percent sign
	static String format(const Timespan& timespan, const String& fmt = "%dd %H:%M:%S.%i");
	static void   append(String& str, const Timespan& timespan, const String& fmt = "%dd %H:%M:%S.%i");

public:
	Timespan();
	Timespan(int64 microseconds);
	Timespan(long seconds, long microseconds);
	Timespan(int days, int hours, int minutes, int seconds, int microseconds);
	Timespan(const Timespan& timespan);
	~Timespan();

	Timespan& operator = (const Timespan& timespan);
	Timespan& operator = (int64 microseconds);

	Timespan& assign(int days, int hours, int minutes, int seconds, int microSeconds);
	Timespan& assign(long seconds, long microseconds);

	void swap(Timespan& timespan);

	int days() const;
	int hours() const;
	int minutes() const;
	int seconds() const;
	int milliseconds() const;
	int microseconds() const;
	int useconds() const;

	int totalHours() const;
	int totalMinutes() const;
	int totalSeconds() const;
	int64 totalMilliseconds() const;
	int64 totalMicroseconds() const;

	bool operator == (const Timespan& ts) const;
	bool operator != (const Timespan& ts) const;
	bool operator >  (const Timespan& ts) const;
	bool operator >= (const Timespan& ts) const;
	bool operator <  (const Timespan& ts) const;
	bool operator <= (const Timespan& ts) const;

	bool operator == (int64 microseconds) const;
	bool operator != (int64 microseconds) const;
	bool operator >  (int64 microseconds) const;
	bool operator >= (int64 microseconds) const;
	bool operator <  (int64 microseconds) const;
	bool operator <= (int64 microseconds) const;

	Timespan  operator +  (const Timespan& d) const;
	Timespan  operator -  (const Timespan& d) const;
	Timespan& operator += (const Timespan& d);
	Timespan& operator -= (const Timespan& d);

	Timespan  operator +  (int64 microseconds) const;
	Timespan  operator -  (int64 microseconds) const;
	Timespan& operator += (int64 microseconds);
	Timespan& operator -= (int64 microseconds);

	operator int64() const { return m_span; }

	static const int64 MILLISECONDS; /// The number of microseconds in a millisecond.
	static const int64 SECONDS;      /// The number of microseconds in a second.
	static const int64 MINUTES;      /// The number of microseconds in a minute.
	static const int64 HOURS;        /// The number of microseconds in a hour.
	static const int64 DAYS;         /// The number of microseconds in a day.

private:
	int64 m_span;
};

//
// inlines
//
inline int Timespan::days() const
{
	return int(m_span / DAYS);
}

inline int Timespan::hours() const
{
	return int((m_span / HOURS) % 24);
}

inline int Timespan::minutes() const
{
	return int((m_span / MINUTES) % 60);
}

inline int Timespan::seconds() const
{
	return int((m_span / SECONDS) % 60);
}

inline int Timespan::milliseconds() const
{
	return int((m_span / MILLISECONDS) % 1000);
}

inline int Timespan::microseconds() const
{
	return int(m_span % 1000);
}

inline int Timespan::useconds() const
{
	return int(m_span % 1000000);
}

inline int Timespan::totalHours() const
{
    return int(m_span / HOURS);
}

inline int Timespan::totalMinutes() const
{
    return int(m_span / MINUTES);
}

inline int Timespan::totalSeconds() const
{
    return int(m_span / SECONDS);
}

inline int64 Timespan::totalMilliseconds() const
{
    return m_span / MILLISECONDS;
}

inline int64 Timespan::totalMicroseconds() const
{
	return m_span;
}

inline bool Timespan::operator == (const Timespan& ts) const
{
	return m_span == ts.m_span;
}

inline bool Timespan::operator != (const Timespan& ts) const
{
	return m_span != ts.m_span;
}

inline bool Timespan::operator >  (const Timespan& ts) const
{
	return m_span > ts.m_span;
}

inline bool Timespan::operator >= (const Timespan& ts) const
{
	return m_span >= ts.m_span;
}

inline bool Timespan::operator <  (const Timespan& ts) const
{
	return m_span < ts.m_span;
}

inline bool Timespan::operator <= (const Timespan& ts) const
{
	return m_span <= ts.m_span;
}

inline bool Timespan::operator == (int64 microseconds) const
{
	return m_span == microseconds;
}

inline bool Timespan::operator != (int64 microseconds) const
{
	return m_span != microseconds;
}

inline bool Timespan::operator >  (int64 microseconds) const
{
	return m_span > microseconds;
}

inline bool Timespan::operator >= (int64 microseconds) const
{
	return m_span >= microseconds;
}

inline bool Timespan::operator <  (int64 microseconds) const
{
	return m_span < microseconds;
}

inline bool Timespan::operator <= (int64 microseconds) const
{
	return m_span <= microseconds;
}

inline void swap(Timespan& s1, Timespan& s2)
{
	s1.swap(s2);
}

CUTE_NS_END
