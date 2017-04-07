#include "Cute/Environment.h"
#include <cstdlib>
#include <cstdio> // sprintf()

#ifdef CUTE_OS_FAMILY_POSIX
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/param.h>
#if defined(CUTE_OS_FAMILY_BSD)
#include <sys/sysctl.h>
#elif CUTE_OS == CUTE_OS_HPUX
#include <pthread.h>
#endif
#endif

CUTE_NS_BEGIN

std::string Environment::get(const std::string& name)
{
	return "";
}

std::string Environment::get(const std::string& name, const std::string& defaultValue)
{
	if (has(name))
		return get(name);
	else
		return defaultValue;
}

bool Environment::has(const std::string& name)
{
	return "";
}

void Environment::set(const std::string& name, const std::string& value)
{
}

std::string Environment::osName()
{
	return "";
}

std::string Environment::osDisplayName()
{
	return "";
}

std::string Environment::osVersion()
{
	return "";
}

std::string Environment::osArchitecture()
{
	return "";
}

std::string Environment::nodeName()
{
	return "";
}

std::string Environment::nodeId()
{
	NodeId id;
	nodeId(id);
	char result[18];
	std::sprintf(result, "%02x:%02x:%02x:%02x:%02x:%02x",
		id[0],
		id[1],
		id[2],
		id[3],
		id[4],
		id[5]);
	return std::string(result);
}


void Environment::nodeId(NodeId& id)
{
}

unsigned Environment::processorCount()
{
#ifdef CUTE_OS_FAMILY_WINDOWS
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwNumberOfProcessors;
#else
#	if defined(_SC_NPROCESSORS_ONLN)
	int count = sysconf(_SC_NPROCESSORS_ONLN);
	if (count <= 0) count = 1;
	return static_cast<int>(count);
#	elif defined(CUTE_OS_FAMILY_BSD)
	unsigned count;
	std::size_t size = sizeof(count);
	if (sysctlbyname("hw.ncpu", &count, &size, 0, 0))
		return 1;
	else
		return count;
#	elif CUTE_OS == CUTE_OS_HPUX
	return pthread_num_processors_np();
#	else
	return 1;
#	endif
#endif
}

CUTE_NS_END