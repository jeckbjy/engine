//! Logging
#include "Cute/LogRotateStrategy.h"
#include "Cute/StringTokenizer.h"
#include "Cute/DateTimeFormatter.h"
#include "Cute/DateTimeParser.h"
#include "Cute/DateTimeFormat.h"
#include "Cute/Exception.h"
#include "Cute/Number.h"
#include "Cute/FileStream.h"
#include "Cute/LogFile.h"

CUTE_NS_BEGIN

//
// RotateStrategy
//
RotateStrategy::RotateStrategy()
{
}


RotateStrategy::~RotateStrategy()
{
}

//
// RotateAtTimeStrategy
//
RotateAtTimeStrategy::RotateAtTimeStrategy(const String& rtime, bool isLocal)
	: m_day(-1)
	, m_hour(-1)
	, m_minute(0)
{
	if (rtime.empty())
		throw InvalidArgumentException("Rotation time must be specified.");

	if ((rtime.find(',') != rtime.npos) && (rtime.find(':') == rtime.npos))
		throw InvalidArgumentException("Invalid rotation time specified.");

	StringTokenizer timestr(rtime, ",:", StringTokenizer::TOK_TRIM | StringTokenizer::TOK_IGNORE_EMPTY);
	int index = 0;

	switch (timestr.count())
	{
	case 3: // day,hh:mm
	{
		String::const_iterator it = timestr[index].begin();
		m_day = DateTimeParser::parseDayOfWeek(it, timestr[index].end());
		++index;
	}
	case 2: // hh:mm
		m_hour = Number::parse(timestr[index]);
		++index;
	case 1: // mm
		m_minute = Number::parse(timestr[index]);
		break;
	default:
		throw InvalidArgumentException("Invalid rotation time specified.");
	}
	getNextRollover();
}

RotateAtTimeStrategy::~RotateAtTimeStrategy()
{
}

bool RotateAtTimeStrategy::mustRotate(LogFile* pFile)
{
	if (DateTime() >= m_threshold)
	{
		getNextRollover();
		return true;
	}

	return false;
}

void RotateAtTimeStrategy::getNextRollover()
{
	Timespan tsp(0, 0, 1, 0, 1000); // 0,00:01:00.001
	do
	{
		m_threshold += tsp;
	} while (!(m_threshold.minute() == m_minute &&
		(-1 == m_hour || m_threshold.hour() == m_hour) &&
		(-1 == m_day || m_threshold.dayOfWeek() == m_day)));
	// round to :00.0 seconds
	m_threshold.assign(m_threshold.year(), m_threshold.month(), m_threshold.day(), m_threshold.hour(), m_threshold.minute());
}

//
// RotateByIntervalStrategy
//
const String RotateByIntervalStrategy::ROTATE_TEXT("# Log file created/rotated ");

RotateByIntervalStrategy::RotateByIntervalStrategy(const Timespan& span) 
	: m_span(span)
	, m_lastRotate(0)
{
	if (span.totalMicroseconds() <= 0) throw InvalidArgumentException("time span must be greater than zero");
}

RotateByIntervalStrategy::~RotateByIntervalStrategy()
{
}

bool RotateByIntervalStrategy::mustRotate(LogFile* pFile)
{
	if (m_lastRotate == 0 || pFile->size() == 0)
	{
		if (pFile->size() != 0)
		{
			std::string tag;
			FileStream fs(pFile->path());
			fs.readLine(tag);
			if (tag.compare(0, ROTATE_TEXT.size(), ROTATE_TEXT) == 0)
			{
				std::string timestamp(tag, ROTATE_TEXT.size());
				int tzd;
				m_lastRotate = DateTimeParser::parse(DateTimeFormat::RFC1036_FORMAT, timestamp, tzd).timestamp();
			}
			else
			{
				m_lastRotate = pFile->creationDate();
			}
		}
		else
		{
			m_lastRotate.update();
			std::string tag(ROTATE_TEXT);
			DateTimeFormatter::append(tag, m_lastRotate, DateTimeFormat::RFC1036_FORMAT);
			pFile->write(tag);
		}
	}
	Timestamp now;
	return m_span <= now - m_lastRotate;
}

//
// RotateBySizeStrategy
//
RotateBySizeStrategy::RotateBySizeStrategy(uint64_t size)
	: m_size(size)
{
	if (size == 0) throw InvalidArgumentException("size must be greater than zero");
}

RotateBySizeStrategy::~RotateBySizeStrategy()
{
}

bool RotateBySizeStrategy::mustRotate(LogFile* pFile)
{
	return pFile->size() >= m_size;
}

CUTE_NS_END