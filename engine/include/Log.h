#pragma once
#include "LogChannel.h"
#include "Mutex.h"
#include "Event.h"
#include "Thread.h"

#include <vector>
#include <queue>
#include <sstream>

CU_NS_BEGIN
//

// 默认的log系统
class CU_API Log
{
public:
	static void LogThread(void* param);

public:
	Log();
	~Log();

	void run();
	void stop();

	void log(const LogMessage& msg);
	void log(const string& text, LogLevel ll, const char* file = 0, int line = 0);
	void log(LogLevel ll, const char* file, int line, const char* fmt, ...);

	void addChannel(LogChannel* channel);
	void setProperty(int type, const std::string& key, const std::string& value);

private:
	void loop();

private:
	typedef std::vector<LogChannel*> ChannelVec;
	typedef std::queue<LogMessage> MessageQueue;

	bool			m_stopped;
	Event			m_event;
	Mutex			m_mutex;
	Thread			m_thread;
	ChannelVec		m_channels;
	MessageQueue	m_messages;
	LogLevel		m_level;		// 输出级别<=的输出
};

extern CU_API Log gLog;

#define LOG_FATAL(fmt, ...) gLog.log(LL_FATAL,__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) gLog.log(LL_ERROR,__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)	gLog.log(LL_WARN,__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) gLog.log(LL_DEBUG,__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_TRACE(fmt, ...) gLog.log(LL_TRACE,__FILE__, __LINE__, fmt, ##__VA_ARGS__)

CU_NS_END