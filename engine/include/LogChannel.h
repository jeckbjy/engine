#pragma once
#include "API.h"
#include <iostream>
#include <fstream>
#include <time.h>

CU_NS_BEGIN

enum LogLevel
{
	LL_FATAL,
	LL_ERROR,
	LL_WARN,
	LL_DEBUG,
	LL_TRACE,
	LL_LOG_MAX,
};

struct LogMessage
{
	std::string	output;		// �������
	std::string	text;		// ԭʼ��Ϣ
	LogLevel	level;		// ����ȼ�
	time_t		timestamp;	// ʱ���
	const char* file;		// �ļ���
	int			line;		// �к�

	LogMessage(LogLevel ll, const char* file, int line);

	const char* levelName() const
	{
		static const char* names[LL_LOG_MAX] = { "FATAL", "ERROR", "WARNING", "DEBUG", "TRACE" };
		return names[level];
	}

	void format();
};

class Log;
class LogChannel
{
public:
	enum
	{
		LOG_NONE,
		LOG_FILE,
		LOG_CONSOLE,
		LOG_SOCKET,
	};

public:
	virtual ~LogChannel(){}
	virtual int type() const = 0;
	virtual void write(const Log* owner, const LogMessage& msg) = 0;
	virtual void flush(){}
	virtual void setProperty(const std::string& /*key*/, const std::string& /*value*/){}

protected:
	LogChannel(){}

private:
	CU_NONCOPYABLE(LogChannel);
};

class ConsoleChannel : public LogChannel
{
private:
	std::ostream& m_stream;

public:
	ConsoleChannel() :m_stream(std::clog){}
	int type() const { return LOG_CONSOLE; }
	void write(const Log* owner, const LogMessage& msg);
};

// todo:�������Զ�����??
class FileChannel : public LogChannel
{
	static const long max_file_size = 1024 * 1024 * 2; //2M
public:
	~FileChannel(){ close(); }
	int type() const { return LOG_FILE; }
	void write(const Log* owner, const LogMessage& msg);
	void flush() { m_file.flush(); }
	void setProperty(const std::string& key, const std::string& value);

private:
	bool open();
	void close();

private:
	std::string m_path;
	std::ofstream m_file;
};

// ���͵�ĳ���ض�������
class SocketLogChannel : public LogChannel
{
public:
	SocketLogChannel();
	~SocketLogChannel();
	int type() const { return LOG_SOCKET; }
	void write(const Log* owner, const LogMessage& msg);
	void setProperty(const std::string& key, const std::string& value);

private:
	socket_t _sock;
};

CU_NS_END