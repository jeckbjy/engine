#pragma once
#include "Types.h"

CU_NS_BEGIN

// 同步通知事件,notify all??
class CU_API SyncEvent
{
public:
	SyncEvent(bool auto_reset = true);
	~SyncEvent();

	void set();
	void reset();
	void wait();
	void wait(long msec);
	bool try_wait(long msec);

	void notify() { set(); }

private:
#ifdef CU_OS_WINNT
	HANDLE			m_handle;
#else
	bool			m_auto;
	volatile bool	m_state;
	pthread_mutex_t m_mutex;
	pthread_cond_t  m_cond;
#endif
};

CU_NS_END