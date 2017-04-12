//! DateTime
#include "Cute/Clock.h"
#include "Cute/Exception.h"
#include "Cute/Timestamp.h"

#if defined(__MACH__)
#include <mach/mach.h>
#include <mach/clock.h>
#elif defined(CUTE_OS_FAMILY_UNIX)
#include <time.h>
#include <unistd.h>
#elif defined(CUTE_VXWORKS)
#include <timers.h>
#endif

#include <algorithm>
#include <limits>

CUTE_NS_BEGIN

const int64_t Clock::CLOCKVAL_MIN = std::numeric_limits<int64_t>::min();
const int64_t Clock::CLOCKVAL_MAX = std::numeric_limits<int64_t>::max();

Clock::Clock()
{
	update();
}

Clock::Clock(int64_t tv)
{
	m_clock = tv;
}

Clock::Clock(const Clock& other)
{
	m_clock = other.m_clock;
}

Clock::~Clock()
{
}

Clock& Clock::operator = (const Clock& other)
{
	m_clock = other.m_clock;
	return *this;
}

Clock& Clock::operator = (int64_t tv)
{
	m_clock = tv;
	return *this;
}

void Clock::swap(Clock& timestamp)
{
	std::swap(m_clock, timestamp.m_clock);
}

void Clock::update()
{
	m_clock = now();
}

bool Clock::monotonic()
{
#if defined(CUTE_OS_FAMILY_WINDOWS) || defined(__MACH__)

	return true;

#elif defined(CUTE_VXWORKS)

#if defined(CUTE_NO_CLOCK_MONOTONIC) // should be in VxWorks 6.x
	return false
#else
	return true;
#endif

#elif defined(_POSIX_TIMERS) && defined(_POSIX_MONOTONIC_CLOCK)

	return true;

#else

	return false;

#endif
}

int64_t Clock::accuracy()
{
#if defined(CUTE_OS_FAMILY_WINDOWS)

	LARGE_INTEGER perfFreq;
	if (QueryPerformanceFrequency(&perfFreq) && perfFreq.QuadPart > 0)
	{
		int64_t acc = 1000000 / perfFreq.QuadPart;
		return acc > 0 ? acc : 1;
	}
	else throw SystemException("cannot get system clock accuracy");

#elif defined(__MACH__)

	clock_serv_t cs;
	int nanosecs;
	mach_msg_type_number_t n = 1;

	host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cs);
	clock_get_attributes(cs, CLOCK_GET_TIME_RES, (clock_attr_t)&nanosecs, &n);
	mach_port_deallocate(mach_task_self(), cs);

	int64_t acc = nanosecs / 1000;
	return acc > 0 ? acc : 1;

#elif defined(CUTE_VXWORKS)

	struct timespec ts;
	if (clock_getres(CLOCK_MONOTONIC, &ts))
		throw SystemException("cannot get system clock");

	int64_t acc = int64_t(ts.tv_sec) * 1000000 + ts.tv_nsec / 1000;
	return acc > 0 ? acc : 1;

#elif defined(_POSIX_TIMERS) && defined(_POSIX_MONOTONIC_CLOCK)

	struct timespec ts;
	if (clock_getres(CLOCK_MONOTONIC, &ts))
		throw SystemException("cannot get system clock");

	int64_t acc = int64_t(ts.tv_sec) * 1000000 + ts.tv_nsec / 1000;
	return acc > 0 ? acc : 1;

#else

	return 1000;

#endif
}

int64_t Clock::now()
{
	int64_t clock;
#if defined(CUTE_OS_FAMILY_WINDOWS)

	LARGE_INTEGER perfCounter;
	LARGE_INTEGER perfFreq;
	if (QueryPerformanceCounter(&perfCounter) && QueryPerformanceFrequency(&perfFreq))
	{
		clock  = 1000000 * (perfCounter.QuadPart / perfFreq.QuadPart);
		clock += (perfCounter.QuadPart % perfFreq.QuadPart) * 1000000 / perfFreq.QuadPart;
	}
	else throw SystemException("cannot get system clock");

#elif defined(__MACH__)

	clock_serv_t cs;
	mach_timespec_t ts;

	host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cs);
	clock_get_time(cs, &ts);
	mach_port_deallocate(mach_task_self(), cs);

	clock = int64_t(ts.tv_sec) * 1000000 + ts.tv_nsec / 1000;

#else
	struct timespec ts;
	if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
		throw SystemException("cannot get system clock");

	clock = int64_t(ts.tv_sec) * 1000000 + ts.tv_nsec / 1000;
#endif

	return clock;
}

CUTE_NS_END