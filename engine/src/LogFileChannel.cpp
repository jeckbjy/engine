//! Logging
#include "Cute/LogFileChannel.h"
#include "Cute/LogArchiveStrategy.h"
#include "Cute/LogRotateStrategy.h"
#include "Cute/LogPurgeStrategy.h"
#include "Cute/DateTimeFormatter.h"
#include "Cute/DateTime.h"
#include "Cute/String.h"
#include "Cute/Number.h"
#include "Cute/Exception.h"
#include "Cute/Ascii.h"

CUTE_NS_BEGIN

const String LogFileChannel::PROP_PATH = "path";
const String LogFileChannel::PROP_ROTATION = "rotation";
const String LogFileChannel::PROP_ARCHIVE = "archive";
const String LogFileChannel::PROP_TIMES = "times";
const String LogFileChannel::PROP_COMPRESS = "compress";
const String LogFileChannel::PROP_PURGEAGE = "purgeAge";
const String LogFileChannel::PROP_PURGECOUNT = "purgeCount";
const String LogFileChannel::PROP_FLUSH = "flush";
const String LogFileChannel::PROP_ROTATEONOPEN = "rotateOnOpen";

LogFileChannel::LogFileChannel(const String& path)
	: m_path(path)
	, m_times("utc")
	, m_rotateOnOpen(false)
	, m_compress(false)
	, m_flush(true)
	, m_file(0)
	, m_rotateStrategy(0)
	, m_purgeStrategy(0)
	, m_archiveStrategy(0)
{

}

LogFileChannel::~LogFileChannel()
{
	try
	{
		close();
		delete m_rotateStrategy;
		delete m_purgeStrategy;
		delete m_archiveStrategy;
	}
	catch (...)
	{
		cute_unexpected();
	}
}

void LogFileChannel::open()
{
	Mutex::ScopedLock lock(m_mutex);

	if (!m_file)
	{
		m_file = new LogFile(m_path);
		if (m_rotateOnOpen && m_file->size() > 0)
		{
			try
			{
				if (m_archiveStrategy)
					m_file = m_archiveStrategy->archive(m_file);
				purge();
			}
			catch (...)
			{
				m_file = new LogFile(m_path);
			}
		}
	}
}

void LogFileChannel::close()
{
	Mutex::ScopedLock lock(m_mutex);

	delete m_file;
	m_file = 0;
}

void LogFileChannel::log(const LogMessage& msg)
{
	open();

	Mutex::ScopedLock lock(m_mutex);

	if (m_rotateStrategy && m_archiveStrategy && m_rotateStrategy->mustRotate(m_file))
	{
		try
		{
			m_file = m_archiveStrategy->archive(m_file);
			purge();
		}
		catch (...)
		{
			m_file = new LogFile(m_path);
		}
		// we must call mustRotate() again to give the
		// RotateByIntervalStrategy a chance to write its timestamp
		// to the new file.
		m_rotateStrategy->mustRotate(m_file);
	}
	m_file->write(msg.getText(), m_flush);
}

void LogFileChannel::setProperty(const String& name, const String& value)
{
	Mutex::ScopedLock lock(m_mutex);

	if (name == PROP_TIMES)
	{
		m_times = value;

		if (!m_rotation.empty())
			setRotation(m_rotation);

		if (!m_archive.empty())
			setArchive(m_archive);
	}
	else if (name == PROP_PATH)
		m_path = value;
	else if (name == PROP_ROTATION)
		setRotation(value);
	else if (name == PROP_ARCHIVE)
		setArchive(value);
	else if (name == PROP_COMPRESS)
		setCompress(value);
	else if (name == PROP_PURGEAGE)
		setPurgeAge(value);
	else if (name == PROP_PURGECOUNT)
		setPurgeCount(value);
	else if (name == PROP_FLUSH)
		setFlush(value);
	else if (name == PROP_ROTATEONOPEN)
		setRotateOnOpen(value);
	else
		LogChannel::setProperty(name, value);
}

String LogFileChannel::getProperty(const String& name) const
{
	if (name == PROP_TIMES)
		return m_times;
	else if (name == PROP_PATH)
		return m_path;
	else if (name == PROP_ROTATION)
		return m_rotation;
	else if (name == PROP_ARCHIVE)
		return m_archive;
	else if (name == PROP_COMPRESS)
		return std::string(m_compress ? "true" : "false");
	else if (name == PROP_PURGEAGE)
		return m_purgeAge;
	else if (name == PROP_PURGECOUNT)
		return m_purgeCount;
	else if (name == PROP_FLUSH)
		return std::string(m_flush ? "true" : "false");
	else if (name == PROP_ROTATEONOPEN)
		return std::string(m_rotateOnOpen ? "true" : "false");
	else
		return LogChannel::getProperty(name);
}

Timestamp LogFileChannel::creationDate() const
{
	if (m_file)
		return m_file->creationDate();
	else
		return 0;
}


uint64_t LogFileChannel::size() const
{
	if (m_file)
		return m_file->size();
	else
		return 0;
}


const std::string& LogFileChannel::path() const
{
	return m_path;
}


void LogFileChannel::setRotation(const std::string& rotation)
{
	std::string::const_iterator it = rotation.begin();
	std::string::const_iterator end = rotation.end();
	int n = 0;
	while (it != end && Ascii::isSpace(*it)) ++it;
	while (it != end && Ascii::isDigit(*it)) { n *= 10; n += *it++ - '0'; }
	while (it != end && Ascii::isSpace(*it)) ++it;
	std::string unit;
	while (it != end && Ascii::isAlpha(*it)) unit += *it++;

	RotateStrategy* pStrategy = 0;
	if ((rotation.find(',') != std::string::npos) || (rotation.find(':') != std::string::npos))
	{
		if (m_times == "utc")
			pStrategy = new RotateAtTimeStrategy(rotation, false);
		else if (m_times == "local")
			pStrategy = new RotateAtTimeStrategy(rotation, true);
		else
			throw PropertyNotSupportedException("times", m_times);
	}
	else if (unit == "daily")
		pStrategy = new RotateByIntervalStrategy(Timespan(1 * Timespan::DAYS));
	else if (unit == "weekly")
		pStrategy = new RotateByIntervalStrategy(Timespan(7 * Timespan::DAYS));
	else if (unit == "monthly")
		pStrategy = new RotateByIntervalStrategy(Timespan(30 * Timespan::DAYS));
	else if (unit == "seconds") // for testing only
		pStrategy = new RotateByIntervalStrategy(Timespan(n*Timespan::SECONDS));
	else if (unit == "minutes")
		pStrategy = new RotateByIntervalStrategy(Timespan(n*Timespan::MINUTES));
	else if (unit == "hours")
		pStrategy = new RotateByIntervalStrategy(Timespan(n*Timespan::HOURS));
	else if (unit == "days")
		pStrategy = new RotateByIntervalStrategy(Timespan(n*Timespan::DAYS));
	else if (unit == "weeks")
		pStrategy = new RotateByIntervalStrategy(Timespan(n * 7 * Timespan::DAYS));
	else if (unit == "months")
		pStrategy = new RotateByIntervalStrategy(Timespan(n * 30 * Timespan::DAYS));
	else if (unit == "K")
		pStrategy = new RotateBySizeStrategy(n * 1024);
	else if (unit == "M")
		pStrategy = new RotateBySizeStrategy(n * 1024 * 1024);
	else if (unit.empty())
		pStrategy = new RotateBySizeStrategy(n);
	else if (unit != "never")
		throw InvalidArgumentException("rotation", rotation);
	delete m_rotateStrategy;
	m_rotateStrategy = pStrategy;
	m_rotation = rotation;
}


void LogFileChannel::setArchive(const std::string& archive)
{
	ArchiveStrategy* pStrategy = 0;
	if (archive == "number")
	{
		pStrategy = new ArchiveByNumberStrategy;
	}
	else if (archive == "timestamp")
	{
		if (m_times == "utc")
			pStrategy = new ArchiveByTimestampStrategy();
		else if (m_times == "local")
			pStrategy = new ArchiveByTimestampStrategy();
		else
			throw PropertyNotSupportedException("times", m_times);
	}
	else throw InvalidArgumentException("archive", archive);
	delete m_archiveStrategy;
	pStrategy->compress(m_compress);
	m_archiveStrategy = pStrategy;
	m_archive = archive;
}


void LogFileChannel::setCompress(const std::string& compress)
{
	m_compress = icompare(compress, "true") == 0;
	if (m_archiveStrategy)
		m_archiveStrategy->compress(m_compress);
}


void LogFileChannel::setPurgeAge(const std::string& age)
{
	if (setNoPurge(age)) return;

	std::string::const_iterator nextToDigit;
	int num = extractDigit(age, &nextToDigit);
	uint64_t factor = extractFactor(age, nextToDigit);

	setPurgeStrategy(new PurgeByAgeStrategy(Timespan(num * factor)));
	m_purgeAge = age;
}


void LogFileChannel::setPurgeCount(const std::string& count)
{
	if (setNoPurge(count)) return;

	setPurgeStrategy(new PurgeByCountStrategy(extractDigit(count)));
	m_purgeCount = count;
}


void LogFileChannel::setFlush(const std::string& flush)
{
	m_flush = icompare(flush, "true") == 0;
}


void LogFileChannel::setRotateOnOpen(const std::string& rotateOnOpen)
{
	m_rotateOnOpen = icompare(rotateOnOpen, "true") == 0;
}


void LogFileChannel::purge()
{
	if (m_purgeStrategy)
	{
		try
		{
			m_purgeStrategy->purge(m_path);
		}
		catch (...)
		{
		}
	}
}


bool LogFileChannel::setNoPurge(const std::string& value)
{
	if (value.empty() || 0 == icompare(value, "none"))
	{
		delete m_purgeStrategy;
		m_purgeStrategy = 0;
		m_purgeAge = "none";
		return true;
	}
	else return false;
}


int LogFileChannel::extractDigit(const std::string& value, std::string::const_iterator* nextToDigit) const
{
	std::string::const_iterator it = value.begin();
	std::string::const_iterator end = value.end();
	int digit = 0;

	while (it != end && Ascii::isSpace(*it)) ++it;
	while (it != end && Ascii::isDigit(*it))
	{
		digit *= 10;
		digit += *it++ - '0';
	}

	if (digit == 0)
		throw InvalidArgumentException("Zero is not valid purge age.");

	if (nextToDigit) *nextToDigit = it;
	return digit;
}


void LogFileChannel::setPurgeStrategy(PurgeStrategy* strategy)
{
	delete m_purgeStrategy;
	m_purgeStrategy = strategy;
}


uint64_t LogFileChannel::extractFactor(const std::string& value, std::string::const_iterator start) const
{
	while (start != value.end() && Ascii::isSpace(*start)) ++start;

	std::string unit;
	while (start != value.end() && Ascii::isAlpha(*start)) unit += *start++;

	if (unit == "seconds")
		return Timespan::SECONDS;
	if (unit == "minutes")
		return Timespan::MINUTES;
	else if (unit == "hours")
		return Timespan::HOURS;
	else if (unit == "days")
		return Timespan::DAYS;
	else if (unit == "weeks")
		return 7 * Timespan::DAYS;
	else if (unit == "months")
		return 30 * Timespan::DAYS;
	else throw InvalidArgumentException("purgeAge", value);

	return 0;
}

CUTE_NS_END