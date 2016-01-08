#include "LogChannel.h"
#include "Console.h"
#include "Util.h"
#include <time.h>

CU_NS_BEGIN

LogMessage::LogMessage(LogLevel ll, const char* file, int line)
{
	this->level = ll;
	this->file = file;
	this->line = line;
}

void LogMessage::format()
{
	char time_str[50];
	timestamp = time(NULL);
	struct tm* timeinfo = localtime(&timestamp);
	strftime(time_str, sizeof(time_str), "%Y/%m/%d %H:%M:%S", timeinfo);
	if (file == NULL)
	{
		Util::vformat(output, "[%s][%s]:%s", time_str, levelName(), text.c_str());
	}
	else
	{
		// 只保留文件名
		size_t len = strlen(file);
		const char* ptr = file + len;
		for (; ptr != file; --ptr)
		{
			if (*ptr == '\\' || *ptr == '/')
			{
				ptr++;
				break;
			}
		}
		Util::vformat(output, "[%s][%s]:%s in %s:%d", time_str, levelName(), text.c_str(), ptr, line);
	}
}

void ConsoleChannel::write(const Log* owner, const LogMessage& msg)
{
	const ColorType colors[LL_LOG_MAX] = { CC_RED, CC_RED, CC_YELLOW, CC_GREEN, CC_LMAGENTA };
	Console::setColor(colors[msg.level]);
	m_stream << msg.output << std::endl;
	Console::setColor();
}

void FileChannel::write(const Log* owner, const LogMessage& msg)
{
	if (!open())
		return;
	std::streamoff size = m_file.tellp();
	if (size > max_file_size)
	{
		m_file.close();
		m_file.open(m_path.c_str(), std::ios::out | std::ios::ate);
	}

	m_file << msg.output << std::endl;
	m_file.flush();
}

void FileChannel::setProperty(const std::string& key, const std::string& value)
{
	if (key == "path")
		m_path = value;
}

bool FileChannel::open()
{
	if (!m_file.is_open())
	{
		if (m_path == "")
		{
			m_path = "./log/";
			m_path += Util::exeName();
			m_path += ".log";
		}
		//int mode = std::ios::out | std::ios::app;
		int mode = std::ios::out;
#ifdef CU_OS_WIN
		m_file.open(m_path.c_str(), mode, _SH_DENYNO);
#else
		m_file.open(m_path.c_str(), mode);
#endif
	}
	return m_file.is_open();
}

void FileChannel::close()
{
	if (m_file.is_open())
		m_file.close();
}

void SocketLogChannel::write(const Log* owner, const LogMessage& msg)
{

}

void SocketLogChannel::setProperty(const std::string& key, const std::string& value)
{

}

CU_NS_END