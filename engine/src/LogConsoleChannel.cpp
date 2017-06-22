//! Logging
#include "Cute/LogConsoleChannel.h"
#include "Cute/Console.h"
#include "Cute/String.h"
#include <iostream>

CUTE_NS_BEGIN

const char* LOG_COLOR_NAME[LOG_LEVEL_MAX] = 
{
	"",
	"fatalColor",
	"criticalColor",
	"errorColor",
	"warningColor",
	"noticeColor",
	"infomationColor",
	"debugColor",
	"traceColor",
};

LogConsoleChannel::LogConsoleChannel()
	: m_str(std::clog)
	, m_enableColors(true)
{
	initColors();
}

LogConsoleChannel::LogConsoleChannel(std::ostream& str)
	: m_str(str)
	, m_enableColors(true)
{
	initColors();
}

LogConsoleChannel::~LogConsoleChannel()
{
}

void LogConsoleChannel::log(const LogMessage& msg)
{
	if (m_enableColors)
		Console::setColor((Console::Color)m_colors[msg.getLevel()]);

	m_str << msg.getText() << std::endl;

	if (m_enableColors)
		Console::resetColor();
}

void LogConsoleChannel::setProperty(const String& name, const String& value)
{
	if (name == "enableColors")
	{
		m_enableColors = icompare(value, "true") == 0;
	}
	else
	{
		for (int i = 1; i < LOG_LEVEL_MAX; ++i)
		{
			if (icompare(name, LOG_COLOR_NAME[i]) == 0)
			{
				m_colors[i] = Console::parseColor(value);
				return;
			}
		}
	}

	LogChannel::setProperty(name, value);
}

String LogConsoleChannel::getProperty(const String& name) const
{
	if (name == "enableColors")
	{
		return m_enableColors ? "true" : "false";
	}
	else
	{
		for (int i = 1; i < LOG_LEVEL_MAX; ++i)
		{
			if (icompare(name, LOG_COLOR_NAME[i]) == 0)
			{
				return Console::formatColor((Console::Color)m_colors[i]);
			}
		}
	}

	return LogChannel::getProperty(name);
}

void LogConsoleChannel::initColors()
{
	m_colors[0]	= Console::DEFAULT; // unused
	m_colors[LOG_LEVEL_FATAL]		= Console::RED;
	m_colors[LOG_LEVEL_CRITICAL]	= Console::RED;
	m_colors[LOG_LEVEL_ERROR]		= Console::RED;
	m_colors[LOG_LEVEL_WARNING]		= Console::YELLOW;
	m_colors[LOG_LEVEL_NOTICE]		= Console::DEFAULT;
	m_colors[LOG_LEVEL_INFORMATION] = Console::DEFAULT;
	m_colors[LOG_LEVEL_DEBUG]		= Console::GRAY;
	m_colors[LOG_LEVEL_TRACE]		= Console::GRAY;
}

CUTE_NS_END
