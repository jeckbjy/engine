#pragma once
#include "Cute/Foundation.h"
#include "Cute/Timestamp.h"

CUTE_NS_BEGIN

enum LogLevel
{
	LOG_LEVEL_FATAL = 1,
	LOG_LEVEL_CRITICAL,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_NOTICE,
	LOG_LEVEL_INFORMATION,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_TRACE,
	LOG_LEVEL_MAX,
};

struct CUTE_CORE_API LogMessage
{
public:
	LogMessage();
	LogMessage(const String& source, const String& text, LogLevel level);
	LogMessage(const String& source, const String& text, LogLevel level, const char* file, int line);
	LogMessage(const LogMessage& other);
	~LogMessage();

	LogMessage& operator = (const LogMessage& msg);

	void swap(LogMessage& msg);
	void setSource(const String& src);
	void setText(const String& text);
	void setThread(const String& name);
	void setTime(const Timestamp& time);
	void setLevel(LogLevel level);
	void setTid(int tid);
	void setPid(long pid);
	void setFile(const char* file);
	void setLine(int line);
	void setMessage(const String& msg) const;

	const String&	getMessage() const;
	const String&	getSource() const;
	const String&	getText() const;
	const String&	getThread() const;
	const Timestamp& getTime() const;

	LogLevel	getLevel() const;
	int			getTid() const;
	long		getPid() const;
	const char* getFile() const;
	int			getLine() const;

protected:
	void init();

private:
	String		m_source;
	String		m_text;
	String		m_thread;
	LogLevel	m_level;
	Timestamp	m_time;
	int			m_tid;
	long		m_pid;
	const char*	m_file;
	int			m_line;
	mutable String m_message;	// 最终序列化后的数据
};

// inlines
inline void LogMessage::setSource(const String& src)
{
	m_source = src;
}

inline void LogMessage::setText(const String& text)
{
	m_text = text;
}

inline void LogMessage::setThread(const String& name)
{
	m_thread = name;
}

inline void LogMessage::setLevel(LogLevel level)
{
	m_level = level;
}

inline void LogMessage::setTime(const Timestamp& time)
{
	m_time = time;
}

inline void LogMessage::setTid(int tid)
{
	m_tid = tid;
}

inline void LogMessage::setPid(long pid)
{
	m_pid = pid;
}

inline void LogMessage::setFile(const char* file)
{
	m_file = file;
}

inline void LogMessage::setLine(int line)
{
	m_line = line;
}

inline const String&  LogMessage::getSource() const
{
	return m_source;
}

inline const String& LogMessage::getText() const
{
	return m_text;
}

inline const String& LogMessage::getThread() const
{
	return m_thread;
}

inline LogLevel LogMessage::getLevel() const
{
	return m_level;
}

inline const Timestamp& LogMessage::getTime() const
{
	return m_time;
}

inline int LogMessage::getTid() const
{
	return m_tid;
}

inline long LogMessage::getPid()  const
{
	return m_pid;
}

inline const char* LogMessage::getFile() const
{
	return m_file;
}

inline int LogMessage::getLine() const
{
	return m_line;
}

inline void LogMessage::setMessage(const String& msg) const
{
	m_message = msg;
}

inline const String& LogMessage::getMessage() const
{
	return m_message.empty() ? m_text : m_message;
}

CUTE_NS_END