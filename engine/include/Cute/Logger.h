#pragma once
#include "Cute/Foundation.h"
#include "Cute/LogChannel.h"
#include "Cute/LogFormatter.h"
#include "Cute/LogStream.h"
#include "Cute/RefPtr.h"
#include "Cute/Ref.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Logger : public SafeRef<Logger>
{
	typedef RefPtr<LogChannel> ChannelPtr;
public:
	static int     parseLevel(const String& level);
	static Logger& root();
	static Logger& get(const String& name);
	static Logger& create(const String& name, LogChannel* channel, int level = LOG_LEVEL_INFORMATION);

public:
	Logger(const String& name, LogChannel* channel, int level);
	~Logger();

	void log(const LogMessage& msg);
	void log(int level, const String& source, const String text, const char* file = NULL, int line = 0);
	bool isEnabled(int level);

	ChannelPtr& getChannel();
	void setChannel(LogChannel* channel);

	int getLevel() const;
	void setLevel(int level);

	void setPattern(const String& pattern);
	const String& getPattern() const;

private:
	String		m_name;
	ChannelPtr	m_channel;
	int			m_level;
	LogFormatter m_formatter;
};

#define LOG_OUTPUT(logger, level, message)				\
	if(logger->isEnabled(level)){						\
		LogStream ss;									\
		ss << message;									\
		logger->log(level, ss.str(), __FILE__, __LINE__);\
	}

#define LOG_DEBUG(logger, message) LOG_OUTPUT(LOG_LEVEL_DEBUG, message)

CUTE_NS_END