//! DateTime
#include "Cute/DateTimeFormat.h"

CUTE_NS_BEGIN

const String DateTimeFormat::ISO8601_FORMAT("%Y-%m-%dT%H:%M:%S%z");
const String DateTimeFormat::ISO8601_FRAC_FORMAT("%Y-%m-%dT%H:%M:%s%z");
const String DateTimeFormat::RFC822_FORMAT("%w, %e %b %y %H:%M:%S %Z");
const String DateTimeFormat::RFC1123_FORMAT("%w, %e %b %Y %H:%M:%S %Z");
const String DateTimeFormat::HTTP_FORMAT("%w, %d %b %Y %H:%M:%S %Z");
const String DateTimeFormat::RFC850_FORMAT("%W, %e-%b-%y %H:%M:%S %Z");
const String DateTimeFormat::RFC1036_FORMAT("%W, %e %b %y %H:%M:%S %Z");
const String DateTimeFormat::ASCTIME_FORMAT("%w %b %f %H:%M:%S %Y");
const String DateTimeFormat::SORTABLE_FORMAT("%Y-%m-%d %H:%M:%S");

const String DateTimeFormat::WEEKDAY_NAMES[] =
{
	"Sunday",
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday"
};

const String DateTimeFormat::MONTH_NAMES[] =
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

CUTE_NS_END
