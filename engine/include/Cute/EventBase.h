#pragma once
#include "Cute/Object.h"
#include "Cute/List.h"

CUTE_NS_BEGIN

/*
1:ֻ���������ȼ�,ͨ��Ϊ��ͨ�������������
2:�����ӳ�ʱ���������ȼ�
3:ʱ����ǵ���clockʱ�����������utc��ʱ���,������Ϣ�ӳٵ���ʹ�ã�������Ϊ��ʱ��ʹ��
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
	int64_t	 m_time;	// millisecond����
};

CUTE_NS_END
