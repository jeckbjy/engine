#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Timezone
{
public:
	/// Returns the offset of local time to UTC, in seconds.
	///	    local time = UTC + utcOffset() + dst().
	static int utcOffset();

	/// Returns the daylight saving time offset in seconds if
	/// daylight saving time is in use.
	///     local time = UTC + utcOffset() + dst().
	static int dst();

	/// Returns true if daylight saving time is in effect
	/// for the given time. Depending on the operating system
	/// platform this might only work reliably for certain
	/// date ranges, as the C library's localtime() function
	/// is used.
	static bool isDst(time_t epochTime);

	/// Returns the time zone differential for the current timezone.
	/// The timezone differential is computed as utcOffset() + dst()
	/// and is expressed in seconds.
	static int tzd();

	/// Returns the timezone name currently in effect.
	static String name();

	/// Returns the timezone name if not daylight saving time is in effect.
	static String standardName();

	/// Returns the timezone name if daylight saving time is in effect.
	static String dstName();
};

CUTE_NS_END