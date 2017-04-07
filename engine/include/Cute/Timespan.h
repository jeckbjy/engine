#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

/// A class that represents time spans up to microsecond resolution.
class CUTE_CORE_API Timespan
{
public:
	Timespan();
	Timespan(int64_t microseconds);
	Timespan(long seconds, long microseconds);
	Timespan(int days, int hours, int minutes, int seconds, int microSeconds);
	Timespan(const Timespan& timespan);
	~Timespan();

	Timespan& operator = (const Timespan& timespan);
	Timespan& operator = (int64_t microseconds);

	Timespan& assign(int days, int hours, int minutes, int seconds, int microSeconds);
	Timespan& assign(long seconds, long microseconds);

	void swap(Timespan& timespan);

	int days() const;

	int hours() const;
	int totalHours() const;

	int minutes() const;
	int totalMinutes() const;

	int seconds() const;
	int totalSeconds() const;

	int milliseconds() const;
	int64_t totalMilliseconds() const;

	int microseconds() const;
	int useconds() const;

	int64_t totalMicroseconds() const;

	bool operator == (const Timespan& ts) const;
	bool operator != (const Timespan& ts) const;
	bool operator >  (const Timespan& ts) const;
	bool operator >= (const Timespan& ts) const;
	bool operator <  (const Timespan& ts) const;
	bool operator <= (const Timespan& ts) const;

	bool operator == (int64_t microSeconds) const;
	bool operator != (int64_t microSeconds) const;
	bool operator >  (int64_t microSeconds) const;
	bool operator >= (int64_t microSeconds) const;
	bool operator <  (int64_t microSeconds) const;
	bool operator <= (int64_t microSeconds) const;

	Timespan  operator +  (const Timespan& d) const;
	Timespan  operator -  (const Timespan& d) const;
	Timespan& operator += (const Timespan& d);
	Timespan& operator -= (const Timespan& d);

	Timespan  operator + (int64_t microSeconds) const;
	Timespan  operator - (int64_t microSeconds) const;
	Timespan& operator += (int64_t microSeconds);
	Timespan& operator -= (int64_t microSeconds);

	operator int64_t() const { return m_span; }

	static const int64_t MILLISECONDS; /// The number of microseconds in a millisecond.
	static const int64_t SECONDS;      /// The number of microseconds in a second.
	static const int64_t MINUTES;      /// The number of microseconds in a minute.
	static const int64_t HOURS;        /// The number of microseconds in a hour.
	static const int64_t DAYS;         /// The number of microseconds in a day.

private:
	int64_t m_span;
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

inline int Timespan::totalHours() const
{
	return int(m_span / HOURS);
}

inline int Timespan::minutes() const
{
	return int((m_span / MINUTES) % 60);
}

inline int Timespan::totalMinutes() const
{
	return int(m_span / MINUTES);
}

inline int Timespan::seconds() const
{
	return int((m_span / SECONDS) % 60);
}

inline int Timespan::totalSeconds() const
{
	return int(m_span / SECONDS);
}

inline int Timespan::milliseconds() const
{
	return int((m_span / MILLISECONDS) % 1000);
}

inline int64_t Timespan::totalMilliseconds() const
{
	return m_span / MILLISECONDS;
}

inline int Timespan::microseconds() const
{
	return int(m_span % 1000);
}

inline int Timespan::useconds() const
{
	return int(m_span % 1000000);
}

inline int64_t Timespan::totalMicroseconds() const
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

inline bool Timespan::operator == (int64_t microSeconds) const
{
	return m_span == microSeconds;
}

inline bool Timespan::operator != (int64_t microSeconds) const
{
	return m_span != microSeconds;
}

inline bool Timespan::operator >  (int64_t microSeconds) const
{
	return m_span > microSeconds;
}

inline bool Timespan::operator >= (int64_t microSeconds) const
{
	return m_span >= microSeconds;
}

inline bool Timespan::operator <  (int64_t microSeconds) const
{
	return m_span < microSeconds;
}

inline bool Timespan::operator <= (int64_t microSeconds) const
{
	return m_span <= microSeconds;
}

inline void swap(Timespan& s1, Timespan& s2)
{
	s1.swap(s2);
}

CUTE_NS_END