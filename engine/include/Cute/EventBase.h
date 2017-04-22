#pragma once
#include "Cute/Object.h"
#include "Cute/List.h"

CUTE_NS_BEGIN

/*
1:只有三个优先级,通常为普通，方便快速排序
2:带有延迟时间的最低优先级
3:时间戳是单调clock时间戳，并不是utc等时间戳,仅做消息延迟调用使用，不能作为定时器使用
*/
class CUTE_CORE_API EventBase : public Object
{
	DECLARE_RTTI(EventBase, Object, OID_ANY);
public:
	enum Priority
	{
		PRIO_LOW = 10,
		PRIO_NORMAL = 20,
		PRIO_HIGH = 30,
	};

	static int64_t now();

	EventBase();
	virtual ~EventBase();

	virtual void process() = 0;
	virtual void destroy();
	virtual bool isValid() const;

	int64_t getTime() const;
	int		getPriority() const;

	void	setDelay(uint32_t delay);
	void	setPriority(int prio);

protected:
	friend class EventQueue;
	ListNode m_hook;
	int		 m_prio;	// Priority
	int64_t	 m_time;	// millisecond毫秒
};

CUTE_NS_END
