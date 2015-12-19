#pragma once
#include "Object.h"
#include "IOOperation.h"

CU_NS_BEGIN
//#define CU_HAS_AIO

class EventLoop;
class CU_API Channel : public Object
{
	DECLARE_RTTI(Channel, Object, "CHAN");
public:
	Channel(EventLoop* loop = NULL);
	virtual ~Channel();
	virtual handle_t handle() const = 0;
	virtual void perform(IOOperation* op) = 0;

	void attach();
	void detach();
	void setEventLoop(EventLoop* loop){ m_loop = loop; }
protected:
	bool m_attached;
	EventLoop* m_loop;
};

CU_NS_END