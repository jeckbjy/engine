#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

/// A Clock stores a monotonic* clock value
/// with (theoretical) microseconds resolution.
/// Clocks can be compared with each other
/// and simple arithmetic is supported.
///
/// [*] Note that Clock values are only monotonic if
/// the operating system provides a monotonic clock.
/// The monotonic() function can be used to check whether
/// the system's clock is monotonic.
///
/// Monotonic Clock is available on Windows, Linux, OS X 
/// and on POSIX platforms supporting clock_gettime() with CLOCK_MONOTONIC.	  
///
/// Clock values are relative to a system-dependent epoch time
/// (usually the system's startup time) and have no relation
/// to the time of day.
class CUTE_CORE_API Clock
{
public:
	static const int64_t RESOLUTION = 1000000;
	static const int64_t CLOCKVAL_MIN; /// Minimum clock value.
	static const int64_t CLOCKVAL_MAX; /// Maximum clock value.

	/// Returns true iff the system's clock is monotonic.
	static bool monotonic();

	/// Returns the system's clock accuracy in microseconds.时间精度？
	static int64_t accuracy();

	/// Return the current time
	static int64_t now();

public:
	Clock();
	Clock(int64_t tv);
	Clock(const Clock& other);
	~Clock();

	Clock& operator = (const Clock& other);
	Clock& operator = (int64_t tv);

	void swap(Clock& clock);
	void update();

	/// Returns the clock value expressed in microseconds
	/// since the system-specific epoch time (usually system
	/// startup).
	int64_t microseconds() const;

	/// Returns the time elapsed since the time denoted by
	/// the Clock instance. Equivalent to Clock() - *this.
	int64_t elapsed() const;

	/// Returns true iff the given interval has passed
	/// since the time denoted by the Clock instance.
	bool isElapsed(int64_t interval) const;

	bool operator == (const Clock& ts) const;
	bool operator != (const Clock& ts) const;
	bool operator >  (const Clock& ts) const;
	bool operator >= (const Clock& ts) const;
	bool operator <  (const Clock& ts) const;
	bool operator <= (const Clock& ts) const;

	Clock  operator +  (int64_t d) const;
	Clock  operator -  (int64_t d) const;
	Clock& operator += (int64_t d);
	Clock& operator -= (int64_t d);

	int64_t operator - (const Clock& ts) const;

	operator int64_t() const { return m_clock; }

private:
	int64_t m_clock;
};

//
// inlines
//
inline void swap(Clock& s1, Clock& s2)
{
	s1.swap(s2);
}

inline int64_t Clock::microseconds() const
{
	return m_clock;
}

inline int64_t Clock::elapsed() const
{
	Clock now;
	return now - *this;
}

inline bool Clock::isElapsed(int64_t interval) const
{
	Clock now;
	int64_t diff = now - *this;
	return diff >= interval;
}

inline bool Clock::operator == (const Clock& ts) const
{
	return m_clock == ts.m_clock;
}

inline bool Clock::operator != (const Clock& ts) const
{
	return m_clock != ts.m_clock;
}

inline bool Clock::operator >  (const Clock& ts) const
{
	return m_clock > ts.m_clock;
}

inline bool Clock::operator >= (const Clock& ts) const
{
	return m_clock >= ts.m_clock;
}

inline bool Clock::operator <  (const Clock& ts) const
{
	return m_clock < ts.m_clock;
}

inline bool Clock::operator <= (const Clock& ts) const
{
	return m_clock <= ts.m_clock;
}

inline Clock Clock::operator + (int64_t d) const
{
	return Clock(m_clock + d);
}

inline Clock Clock::operator - (int64_t d) const
{
	return Clock(m_clock - d);
}

inline int64_t Clock::operator - (const Clock& ts) const
{
	return m_clock - ts.m_clock;
}

inline Clock& Clock::operator += (int64_t d)
{
	m_clock += d;
	return *this;
}

inline Clock& Clock::operator -= (int64_t d)
{
	m_clock -= d;
	return *this;
}

CUTE_NS_END
