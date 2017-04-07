// module DateTime
#include "Cute/Timezone.h"
#include "Cute/Exception.h"
#include <ctime>

CUTE_NS_BEGIN

#ifdef CUTE_OS_FAMILY_UNIX
class TZInfo
{
public:
	TZInfo()
	{
		tzset();
	}

	int timeZone()
	{
#if defined(__APPLE__)  || defined(__FreeBSD__) || defined (__OpenBSD__) || defined(__ANDROID__) 
		// no timezone global var
		std::time_t now = std::time(NULL);
		struct std::tm t;
		gmtime_r(&now, &t);
		std::time_t utc = std::mktime(&t);
		return now - utc;
#elif defined(__CYGWIN__)
		tzset();
		return -_timezone;
#else
		tzset();
		return -timezone;
#endif
	}

	const char* name(bool dst)
	{
		tzset();
		return tzname[dst ? 1 : 0];
	}
};

static TZInfo tzInfo;
#endif

int Timezone::utcOffset()
{
#if defined(_WIN32)
	TIME_ZONE_INFORMATION tzInfo;
	DWORD dstFlag = GetTimeZoneInformation(&tzInfo);
	return -tzInfo.Bias * 60;
#elif defined(CUTE_OS_FAMILY_UNIX)
	return tzInfo.timeZone();
#endif
}

int Timezone::dst()
{
#if defined(_WIN32)
	TIME_ZONE_INFORMATION tzInfo;
	DWORD dstFlag = GetTimeZoneInformation(&tzInfo);
	return dstFlag == TIME_ZONE_ID_DAYLIGHT ? -tzInfo.DaylightBias * 60 : 0;

#elif defined(CUTE_OS_FAMILY_UNIX)

	std::time_t now = std::time(NULL);
	struct std::tm t;
	if (!localtime_r(&now, &t))
		throw SystemException("cannot get local time DST offset");
	return t.tm_isdst == 1 ? 3600 : 0;
#endif
}

bool Timezone::isDst(time_t epochTime)
{
	struct std::tm* tms = std::localtime(&epochTime);
	if (!tms)
		throw SystemException("cannot get local time DST flag");
	return tms->tm_isdst > 0;
}

int Timezone::tzd()
{
	return utcOffset() + dst();
}

String Timezone::name()
{
#if defined(_WIN32)
	String result;
	TIME_ZONE_INFORMATION tzInfo;
	DWORD dstFlag = GetTimeZoneInformation(&tzInfo);
	WCHAR* ptr = dstFlag == TIME_ZONE_ID_DAYLIGHT ? tzInfo.DaylightName : tzInfo.StandardName;
#if defined(POCO_WIN32_UTF8)
	UnicodeConverter::toUTF8(ptr, result);
#else
	char buffer[256];
	DWORD rc = WideCharToMultiByte(CP_ACP, 0, ptr, -1, buffer, sizeof(buffer), NULL, NULL);
	if (rc) result = buffer;
#endif
	return result;

#elif defined(CUTE_OS_FAMILY_UNIX)

	return tzInfo.name(dst() != 0);
#endif
}

String Timezone::standardName()
{
#if defined(_WIN32)
	String result;
	TIME_ZONE_INFORMATION tzInfo;
	DWORD dstFlag = GetTimeZoneInformation(&tzInfo);
	WCHAR* ptr = tzInfo.StandardName;
#if defined(POCO_WIN32_UTF8)
	UnicodeConverter::toUTF8(ptr, result);
#else
	char buffer[256];
	DWORD rc = WideCharToMultiByte(CP_ACP, 0, ptr, -1, buffer, sizeof(buffer), NULL, NULL);
	if (rc) result = buffer;
#endif
	return result;
#elif defined(CUTE_OS_FAMILY_UNIX)

	return tzInfo.name(false);
#endif
}

String Timezone::dstName()
{
#if defined(_WIN32)
	std::string result;
	TIME_ZONE_INFORMATION tzInfo;
	DWORD dstFlag = GetTimeZoneInformation(&tzInfo);
	WCHAR* ptr = tzInfo.DaylightName;
#if defined(POCO_WIN32_UTF8)
	UnicodeConverter::toUTF8(ptr, result);
#else
	char buffer[256];
	DWORD rc = WideCharToMultiByte(CP_ACP, 0, ptr, -1, buffer, sizeof(buffer), NULL, NULL);
	if (rc) result = buffer;
#endif
	return result;

#elif defined(CUTE_OS_FAMILY_UNIX)

	return tzInfo.name(true);
#endif
}

CUTE_NS_END
