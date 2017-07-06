//! Process
#include "Cute/Pipe.h"
#include <stdexcept>

#ifdef CUTE_OS_FAMILY_POSIX
#	include <sys/types.h>
#	include <unistd.h>
#	include <errno.h>
#endif

CUTE_NS_BEGIN

Pipe::Pipe()
{
#if defined(_WIN32)
	SECURITY_ATTRIBUTES attr;
	attr.nLength = sizeof(attr);
	attr.lpSecurityDescriptor = NULL;
	attr.bInheritHandle = FALSE;
	if (!CreatePipe(&m_reader, &m_writer, &attr, 0))
		throw std::exception("anonymous pipe");
#else
	int fds[2];
	int rc = pipe(fds);
	if (rc == 0)
	{
		m_reader = fds[0];
		m_writer = fds[1];
	}
	else throw std::runtime_error("create pipe error");
#endif
}

Pipe::~Pipe()
{
	close(CLOSE_BOTH);
}

long Pipe::write(const void* buffer, int length)
{
#if defined(_WIN32)
	assert(m_writer != INVALID_HANDLE_VALUE);

	DWORD bytes = 0;
	if (!WriteFile(m_writer, buffer, length, &bytes, NULL))
		throw std::exception("Pipe WriteFile Error");

	return bytes;
#else

	assert(m_writer != -1);
	long n;
	do
	{
		n = ::write(m_writer, buffer, length);
	} while (n < 0 && errno == EINTR);

	if (n >= 0)
		return n;
	else
		throw std::runtime_error("pipe write error");

#endif
}

long Pipe::read(void* buffer, int length)
{
#if defined(_WIN32)
	assert(m_reader != INVALID_HANDLE_VALUE);

	DWORD bytesRead = 0;
	BOOL ok = ReadFile(m_reader, buffer, length, &bytesRead, NULL);
	if (ok || GetLastError() == ERROR_BROKEN_PIPE)
		return bytesRead;
	else
		throw std::runtime_error("Pipe ReadFile error");
#else
	assert(m_reader != -1);

	long n;
	do
	{
		n = ::read(m_reader, buffer, length);
	} while (n < 0 && errno == EINTR);
	if (n >= 0)
		return n;
	else
		throw std::runtime_error("pipe read error");
#endif
}

void Pipe::close(CloseMode mode /* = CLOSE_BOTH */)
{
	switch (mode)
	{
	case CLOSE_READ:
		CUTE_CLOSE_HANDLE(m_reader);
		break;
	case CLOSE_WRITE:
		CUTE_CLOSE_HANDLE(m_writer);
		break;
	case CLOSE_BOTH:
		CUTE_CLOSE_HANDLE(m_reader);
		CUTE_CLOSE_HANDLE(m_writer);
		break;
	}
}

CUTE_NS_END
