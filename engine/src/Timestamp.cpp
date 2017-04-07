// module DateTime
#include "Cute/Timestamp.h"

CUTE_NS_BEGIN

int64_t Timestamp::now()
{
#if defined(CUTE_OS_FAMILY_WINDOWS)

	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);

	ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
	epoch.LowPart = 0xD53E8000;
	epoch.HighPart = 0x019DB1DE;

	ULARGE_INTEGER ts;
	ts.LowPart = ft.dwLowDateTime;
	ts.HighPart = ft.dwHighDateTime;
	ts.QuadPart -= epoch.QuadPart;
	return ts.QuadPart / 10;

#else
	struct timespec ts;
	if (clock_gettime(CLOCK_REALTIME, &ts))
		throw SystemException("cannot get time of day");

	return int64_t(ts.tv_sec) * 1000000 + ts.tv_nsec / 1000;
#endif
}

Timestamp Timestamp::fromEpochTime(time_t t)
{
	return Timestamp(int64_t(t) * RESOLUTION);
}

Timestamp Timestamp::fromUtcTime(int64_t val)
{
	val -= (int64_t(0x01b21dd2) << 32) + 0x13814000;
	val /= 10;
	return Timestamp(val);
}

Timestamp::Timestamp()
{
	update();
}

Timestamp::Timestamp(int64_t ts)
{
	m_ts = ts;
}

Timestamp::Timestamp(const Timestamp& other)
{
	m_ts = other.m_ts;
}

Timestamp::~Timestamp()
{
}

Timestamp& Timestamp::operator =(const Timestamp& other)
{
	m_ts = other.m_ts;
	return *this;
}

Timestamp& Timestamp::operator =(int64_t ts)
{
	m_ts = ts;
	return *this;
}

void Timestamp::swap(Timestamp& other)
{
	std::swap(m_ts, other.m_ts);
}

void Timestamp::update()
{
	m_ts = now();
}

CUTE_NS_END
