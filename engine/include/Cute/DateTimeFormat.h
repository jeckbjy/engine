#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

/// Definition of date/time formats and various
/// constants used by DateTimeFormatter and DateTimeParser.
class CUTE_CORE_API DateTimeFormat
{
public:
	// predefined date formats
	static const String ISO8601_FORMAT;
	/// The date/time format defined in the ISO 8601 standard.
	///
	/// Examples: 
	///   2005-01-01T12:00:00+01:00
	///   2005-01-01T11:00:00Z

	static const String ISO8601_FRAC_FORMAT;
	/// The date/time format defined in the ISO 8601 standard,
	/// with fractional seconds.
	///
	/// Examples: 
	///   2005-01-01T12:00:00.000000+01:00
	///   2005-01-01T11:00:00.000000Z

	static const String RFC822_FORMAT;
	/// The date/time format defined in RFC 822 (obsoleted by RFC 1123).
	///
	/// Examples: 
	///   Sat, 1 Jan 05 12:00:00 +0100
	///   Sat, 1 Jan 05 11:00:00 GMT

	static const String RFC1123_FORMAT;
	/// The date/time format defined in RFC 1123 (obsoletes RFC 822).
	///
	/// Examples: 
	///   Sat, 1 Jan 2005 12:00:00 +0100
	///   Sat, 1 Jan 2005 11:00:00 GMT

	static const String HTTP_FORMAT;
	/// The date/time format defined in the HTTP specification (RFC 2616),
	/// which is basically a variant of RFC 1036 with a zero-padded day field.
	///
	/// Examples: 
	///   Sat, 01 Jan 2005 12:00:00 +0100
	///   Sat, 01 Jan 2005 11:00:00 GMT

	static const String RFC850_FORMAT;
	/// The date/time format defined in RFC 850 (obsoleted by RFC 1036).
	///
	/// Examples: 
	///   Saturday, 1-Jan-05 12:00:00 +0100
	///   Saturday, 1-Jan-05 11:00:00 GMT

	static const String RFC1036_FORMAT;
	/// The date/time format defined in RFC 1036 (obsoletes RFC 850).
	///
	/// Examples: 
	///   Saturday, 1 Jan 05 12:00:00 +0100
	///   Saturday, 1 Jan 05 11:00:00 GMT

	static const String ASCTIME_FORMAT;
	/// The date/time format produced by the ANSI C asctime() function.
	///
	/// Example: 
	///   Sat Jan  1 12:00:00 2005

	static const String SORTABLE_FORMAT;
	/// A simple, sortable date/time format.
	///
	/// Example:
	///   2005-01-01 12:00:00


	// names used by formatter and parser
	static const String WEEKDAY_NAMES[7];
	/// English names of week days (Sunday, Monday, Tuesday, ...).

	static const String MONTH_NAMES[12];
	/// English names of months (January, February, ...).	
};

CUTE_NS_END