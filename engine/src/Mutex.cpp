#include "Mutex.h"

CU_NS_BEGIN

Mutex::Mutex(bool recursive /* = false */)
{
#ifdef CU_OS_POSIX
	if (recursive)
	{
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		mutex_init(&m_handle, &attr);
	}
	else
	{
		mutex_init(&m_handle);
	}
#else
	mutex_init(&m_handle);
#endif
}

CU_NS_END