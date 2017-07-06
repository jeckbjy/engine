#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

// 管道
class CUTE_CORE_API Pipe
{
public:
	enum CloseMode
	{
		CLOSE_READ = 0x01,
		CLOSE_WRITE = 0x02,
		CLOSE_BOTH = 0x03,
	};

public:
	Pipe();
	~Pipe();

	long write(const void* buffer, int length);
	long read(void* buffer, int length);
	void close(CloseMode mode = CLOSE_BOTH);

	inline handle_t getReader() const { return m_reader; }
	inline handle_t getWriter() const { return m_writer; }

private:
	Pipe(const Pipe& pipe);
	Pipe& operator = (const Pipe& pipe);

private:
	handle_t m_reader;
	handle_t m_writer;
};

CUTE_NS_END
