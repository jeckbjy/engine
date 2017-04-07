//! Logging
#include "Cute/Logger.h"
#include "Cute/Number.h"
#include "Cute/String.h"
#include "Cute/Exception.h"
#include "Cute/Mutex.h"

CUTE_NS_BEGIN

const String LOGGER_ROOT;

class LoggerGroup
{
public:
	LoggerGroup()
	{

	}

	~LoggerGroup()
	{
		destory();
	}

	Logger* find(const String& name)
	{
		LoggerMap::iterator itor = m_loggers.find(name);
		if (itor != m_loggers.end())
			return itor->second;

		return 0;
	}

	Logger& getOrCreate(const String& name)
	{
		Logger* logger = find(name);
		if (!logger)
		{
			if (name == LOGGER_ROOT)
			{
				logger = new Logger(name, 0, LOG_LEVEL_INFORMATION);
			}
			else
			{
				Logger& par = parent(name);
				logger = new Logger(name, par.getChannel(), par.getLevel());
			}

			m_loggers[name] = logger;
		}

		return *logger;
	}

	Logger& parent(const String& name)
	{
		std::string::size_type pos = name.rfind('.');
		if (pos != std::string::npos)
		{
			std::string pname = name.substr(0, pos);
			Logger* pParent = find(pname);
			if (pParent)
				return *pParent;
			else
				return parent(pname);
		}
		else
		{
			return root();
		}
	}

	Logger& get(const String& name)
	{
		Mutex::ScopedLock lock(m_mutex);
		return getOrCreate(name);
	}

	Logger& root()
	{
		return get(LOGGER_ROOT);
	}

	void destory()
	{
		Mutex::ScopedLock lock(m_mutex);
		for (LoggerMap::iterator itor = m_loggers.begin(); itor != m_loggers.end(); ++itor)
		{
			delete itor->second;
		}
		m_loggers.clear();
	}

private:
	typedef std::map<String, Logger*> LoggerMap;
	LoggerMap	m_loggers;
	Mutex		m_mutex;
};

namespace{
	LoggerGroup gLoggerGroup;
}

int Logger::parseLevel(const String& level)
{
	if (icompare(level, "none") == 0)
		return 0;
	else if (icompare(level, "fatal") == 0)
		return LOG_LEVEL_FATAL;
	else if (icompare(level, "critical") == 0)
		return LOG_LEVEL_CRITICAL;
	else if (icompare(level, "error") == 0)
		return LOG_LEVEL_ERROR;
	else if (icompare(level, "warning") == 0)
		return LOG_LEVEL_WARNING;
	else if (icompare(level, "notice") == 0)
		return LOG_LEVEL_NOTICE;
	else if (icompare(level, "information") == 0)
		return LOG_LEVEL_INFORMATION;
	else if (icompare(level, "debug") == 0)
		return LOG_LEVEL_DEBUG;
	else if (icompare(level, "trace") == 0)
		return LOG_LEVEL_TRACE;
	else
	{
		int numLevel;
		if (Number::tryParse(level, numLevel))
		{
			if (numLevel > 0 && numLevel < 9)
				return numLevel;
			else
				throw InvalidArgumentException("Log level out of range ", level);
		}
		else
			throw InvalidArgumentException("Not a valid log level", level);
	}
}

Logger& Logger::root()
{
	return gLoggerGroup.root();
}

Logger& Logger::get(const String& name)
{
	return gLoggerGroup.get(name);
}

//Logger& Logger::create(const String& name, LogChannel* channel, int level /* = LOG_LEVEL_INFORMATION */)
//{
//	//return gLoggerGroup.getOrCreate();
//}

Logger::Logger(const String& name, LogChannel* channel, int level)
	: m_name(name)
	, m_channel(channel)
	, m_level(level)
{
}

Logger::~Logger()
{
}

void Logger::log(const LogMessage& msg)
{
	if (m_channel)
	{
		m_formatter.format(msg);
		m_channel->log(msg);
	}
}

void Logger::log(int level, const String& source, const String text, const char* file /* = NULL */, int line /* = 0 */)
{
	if (m_channel)
	{
		LogMessage msg(source, text, (LogLevel)level, file, line);
		m_formatter.format(msg);
		m_channel->log(msg);
	}
}

bool Logger::isEnabled(int level)
{
	return level <= m_level;
}

Logger::ChannelPtr& Logger::getChannel()
{
	return m_channel;
}

void Logger::setChannel(LogChannel* channel)
{
	m_channel = channel;
}

int Logger::getLevel() const
{
	return m_level;
}

void Logger::setLevel(int level)
{
	m_level = level;
}

void Logger::setPattern(const String& pattern)
{
	m_formatter.setPattern(pattern);
}

const String& Logger::getPattern() const
{
	return m_formatter.getPattern();
}

CUTE_NS_END