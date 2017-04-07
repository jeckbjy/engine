#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

// http://en.cppreference.com/w/c/chrono/timespec_get
// http://stackoverflow.com/questions/5404277/porting-clock-gettime-to-windows
// http://blog.csdn.net/rangf/article/details/6739121

/// A Timestamp stores a monotonic* time value
/// with (theoretical) microseconds resolution.
/// Timestamps can be compared with each other
/// and simple arithmetic is supported.
///
/// [*] Note that Timestamp values are only monotonic as
/// long as the systems's clock is monotonic as well
/// (and not, e.g. set back due to time synchronization
/// or other reasons).
///
/// Timestamps are UTC (Coordinated Universal Time)
/// based and thus independent of the timezone
/// in effect on the system.
///
/// The internal reference time is the Unix epoch, 
/// midnight, January 1, 1970.
class CUTE_CORE_API Timestamp
{
public:
	/// Returns the resolution in units per second.
	/// Since the timestamp has microsecond resolution,
	/// the returned value is always 1000000.
	static const int64_t RESOLUTION = 1000000;

	static int64_t	 now();
	static int64_t	 resolution() { return RESOLUTION; }
	static Timestamp fromEpochTime(time_t t);
	static Timestamp fromUtcTime(int64_t val);

	Timestamp();
	Timestamp(int64_t ts);
	Timestamp(const Timestamp& other);
	~Timestamp();

	Timestamp& operator=(const Timestamp& other);
	Timestamp& operator=(int64_t tv);

	void swap(Timestamp& other);
	void update();

	/// Returns the time elapsed since the time denoted by
	/// the timestamp. Equivalent to Timestamp() - *this.
	int64_t elapsed() const;
	/// Returns true iff the given interval has passed
	/// since the time denoted by the timestamp.
	bool isElapsed(int64_t interval) const;

	time_t	 epochTime() const;
	int64_t  utcTime() const;
	int64_t  epochMicroseconds() const;

	bool operator == (const Timestamp& ts) const;
	bool operator != (const Timestamp& ts) const;
	bool operator >  (const Timestamp& ts) const;
	bool operator >= (const Timestamp& ts) const;
	bool operator <  (const Timestamp& ts) const;
	bool operator <= (const Timestamp& ts) const;

	Timestamp  operator +  (int64_t d) const;
	Timestamp  operator -  (int64_t d) const;
	Timestamp& operator += (int64_t d);
	Timestamp& operator -= (int64_t d);

	int64_t   operator -  (const Timestamp& ts) const;
	//operator int64_t() const { return m_ts; }

private:
	int64_t m_ts;
};

//////////////////////////////////////////////////////////////////////////
// inlines
//////////////////////////////////////////////////////////////////////////
inline int64_t Timestamp::elapsed() const
{
	Timestamp now;
	return now - *this;
}

inline bool Timestamp::isElapsed(int64_t interval) const
{
	Timestamp now;
	int64_t diff = now - *this;
	return diff >= interval;
}

inline time_t Timestamp::epochTime() const
{
	return time_t(m_ts / RESOLUTION);
}

inline int64_t Timestamp::utcTime() const
{
	return m_ts * 10 + (int64_t(0x01b21dd2) << 32) + 0x13814000;
}

inline int64_t Timestamp::epochMicroseconds() const
{
	return m_ts;
}

inline bool Timestamp::operator == (const Timestamp& ts) const
{
	return m_ts == ts.m_ts;
}

inline bool Timestamp::operator != (const Timestamp& ts) const
{
	return m_ts != ts.m_ts;
}

inline bool Timestamp::operator >  (const Timestamp& ts) const
{
	return m_ts > ts.m_ts;
}

inline bool Timestamp::operator >= (const Timestamp& ts) const
{
	return m_ts >= ts.m_ts;
}

inline bool Timestamp::operator <  (const Timestamp& ts) const
{
	return m_ts < ts.m_ts;
}

inline bool Timestamp::operator <= (const Timestamp& ts) const
{
	return m_ts <= ts.m_ts;
}

inline Timestamp Timestamp::operator + (int64_t d) const
{
	return Timestamp(m_ts + d);
}

inline Timestamp Timestamp::operator - (int64_t d) const
{
	return Timestamp(m_ts - d);
}

inline Timestamp& Timestamp::operator += (int64_t d)
{
	m_ts += d;
	return *this;
}

inline Timestamp& Timestamp::operator -= (int64_t d)
{
	m_ts -= d;
	return *this;
}

inline int64_t Timestamp::operator - (const Timestamp& ts) const
{
	return m_ts - ts.m_ts;
}

CUTE_NS_END
