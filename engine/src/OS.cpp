#include "Cute/OS.h"
#include "Cute/Exception.h"

CUTE_NS_BEGIN

#ifdef CUTE_OS_FAMILY_WINDOWS

void ConvertToSystem(const String& src, TString& dst)
{
#ifdef CUTE_WIN32_UTF8
	// utf8 to wide
	DWORD len = ::MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, NULL, 0);
	if (len > 0)
	{
		dst.resize(len);
		::MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, &dst[0], len);
	}
#else
	// local ascii code
	dst = src;
#endif
}

void ConvertToLocal(const TCHAR* src, String& dst)
{
#ifdef CUTE_WIN32_UTF8
	// wchar to utf8
	DWORD len = ::WideCharToMultiByte(CP_UTF8, 0, src, -1, NULL, 0, NULL, NULL);
	if (len > 0)
	{
		dst.resize(len);
		::WideCharToMultiByte(CP_UTF8, 0, src, -1, &dst[0], len, NULL, NULL);
	}
#else
	// local ascii code
	dst = src;
#endif
}

void ConvertWideToLocal(const WCHAR* src, String& dst)
{
	UINT codePage;
#ifdef CUTE_WIN32_UTF8
	codePage = CP_UTF8;
#else
	codePage = CP_ACP;
#endif

	DWORD len = ::WideCharToMultiByte(codePage, 0, src, -1, NULL, 0, NULL, NULL);
	if (len > 0)
	{
		dst.resize(len);
		::WideCharToMultiByte(codePage, 0, src, -1, &dst[0], len, NULL, NULL);
	}
}

void ConvertUTF8ToWide(const String& src, WString& dst)
{
	DWORD len = ::MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, NULL, 0);
	if (len > 0)
	{
		dst.resize(len);
		::MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, &dst[0], len);
	}
}

void ConvertWideToUTF8(const WCHAR* src, String& dst)
{
	UINT codePage = CP_UTF8;
	DWORD len = ::WideCharToMultiByte(codePage, 0, src, -1, NULL, 0, NULL, NULL);
	if (len > 0)
	{
		dst.resize(len);
		::WideCharToMultiByte(codePage, 0, src, -1, &dst[0], len, NULL, NULL);
	}
}

uint64_t ConvertTimeToEngine(DWORD timeLow, DWORD timeHigh)
{
	ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
	epoch.LowPart = 0xD53E8000;
	epoch.HighPart = 0x019DB1DE;

	ULARGE_INTEGER ts;
	ts.LowPart = timeLow;
	ts.HighPart = timeHigh;
	ts.QuadPart -= epoch.QuadPart;

	return ts.QuadPart / 10;
}

void ConvertTimeToSystem(uint64_t timestamp, DWORD& timeLow, DWORD& timeHigh)
{
	ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
	epoch.LowPart = 0xD53E8000;
	epoch.HighPart = 0x019DB1DE;

	ULARGE_INTEGER ts;
	ts.QuadPart = timestamp * 10;
	ts.QuadPart += epoch.QuadPart;
	timeLow = ts.LowPart;
	timeHigh = ts.HighPart;
}

#endif

#if defined(CUTE_EMULATE_CLOCK_GETTIME)

#if defined(__MACH__)
#define MT_NANO (+1.0E-9)
#define MT_GIGA UINT64_C(1000000000)

// TODO create a list of timers,
static double   mt_timebase = 0.0;
static uint64_t mt_timestart = 0;

#endif

// 模拟实现：clock_gettime
long clock_gettime(int clk_id, struct timespec* tp)
{
#ifdef _WIN32
	switch (clk_id)
	{
	case CLOCK_REALTIME:
	case CLOCK_REALTIME_HR:
	{
		FILETIME ft;
		GetSystemTimeAsFileTime(&ft);
		ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
		epoch.LowPart = 0xD53E8000;
		epoch.HighPart = 0x019DB1DE;

		ULARGE_INTEGER ts;
		ts.LowPart = ft.dwLowDateTime;
		ts.HighPart = ft.dwHighDateTime;
		ts.QuadPart -= epoch.QuadPart;

		tp->tv_sec  = ts.QuadPart / 10000000i64;
		tp->tv_nsec = ts.QuadPart % 10000000i64 * 100;
		break;
	}
	case CLOCK_MONOTONIC:
	case CLOCK_MONOTONIC_HR:
	{
		LARGE_INTEGER perfCounter;
		LARGE_INTEGER perfFreq;
		if (QueryPerformanceCounter(&perfCounter) && QueryPerformanceFrequency(&perfFreq))
		{
			ULONGLONG clock;
			clock = 1000000 * (perfCounter.QuadPart / perfFreq.QuadPart);
			clock += (perfCounter.QuadPart % perfFreq.QuadPart) * 1000000 / perfFreq.QuadPart;

			tp->tv_sec  = clock / 10000000i64;
			tp->tv_nsec = clock % 10000000i64 * 100;
		}
		else throw SystemException("cannot get system clock");
		break;
	}
	case CLOCK_PROCESS_CPUTIME_ID:
	{// todo:
		return -EINVAL;
		break;
	}
	case CLOCK_THREAD_CPUTIME_ID:
	{// todo:
		return -EINVAL;
		break;
	}
	default:
	{
		return -EINVAL;
	}
	}

	return 0;
#elif defined(__MACH__)
	kern_return_t retval = KERN_SUCCESS;

	if (clk_id == TIMER_ABSTIME)
	{
		if (!mt_timestart) { // only one timer, initilized on the first call to the TIMER
			mach_timebase_info_data_t tb = { 0 };
			mach_timebase_info(&tb);
			mt_timebase = tb.numer;
			mt_timebase /= tb.denom;
			mt_timestart = mach_absolute_time();
		}

		double diff = (mach_absolute_time() - mt_timestart) * mt_timebase;
		tp->tv_sec = diff * MT_NANO;
		tp->tv_nsec = diff - (tp->tv_sec * MT_GIGA);
	}
	else // other clk_ids are mapped to the coresponding mach clock_service
	{
		clock_serv_t cclock;
		mach_timespec_t mts;

		host_get_clock_service(mach_host_self(), clk_id, &cclock);
		retval = clock_get_time(cclock, &mts);
		mach_port_deallocate(mach_task_self(), cclock);

		tp->tv_sec = mts.tv_sec;
		tp->tv_nsec = mts.tv_nsec;
	}

	return retval;
#else
	struct timeval tv;
	if (gettimeofday(&tv, NULL))
		throw SystemException("cannot get time of day");

	tp->tv_sec  = tv.tv_sec;
	tp->tv_nsec = tv.tv_usec * 1000;

	return 0;
#endif

}
#endif

CUTE_NS_END
