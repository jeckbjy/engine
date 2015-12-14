#pragma once
#include "Ref.h"
#include "Rtti.h"
#include "List.h"
#include "FuncTraits.h"

CU_NS_BEGIN

class Object;
struct CU_API Event
{
	DECLARE_RTTI(Event, RootRtti, "EVEN");
public:
	Event():sender(NULL){}
	virtual ~Event(){}
	Object* sender;
};

struct EventHandler : public ListNode2<EventHandler>, public Ref
{
public:
	EventHandler(uint32_t type, Object* owner, Object* sender, void* udata)
		:m_event(type), m_owner(owner), m_sender(sender), m_udata(udata){}
	virtual void invoke(Event* event) = 0;

	bool equal(uint32_t type) const { return m_event == type; }
	bool equal(uint32_t type, const Object* sender) const { return m_event == type && m_sender == sender; }
	uint32_t getType() const { return m_event; }
	Object* getObject() const { return m_owner; }
	Object* getReceiver() const { return m_owner; }
	Object* getSender() const { return m_sender; }
	template<typename T>
	T* getData() const { return (T*)m_udata; }

protected:
	uint32_t	m_event;	// 事件类型
	Object*		m_owner;
	Object*		m_sender;
	void*		m_udata;
};

template<typename F>
struct TEventHandler : public EventHandler
{
	typedef F func_t;
	typedef typename func_traits<F>::class_t  class_t;
	typedef typename func_traits<F>::param0_t param0_t;
	typedef typename std::remove_pointer<param0_t>::type event_t;

public:
	TEventHandler(func_t fun, uint32_t type, Object* owner, Object* sender, void* udata) :EventHandler(type, owner, sender, udata), m_fun(fun){}

	void invoke(Event* event) 
	{
		class_t* obj = (class_t*)m_owner;
		event_t* ev = cast<event_t, Event>(event);
		if (ev)
			(obj->*m_fun)(ev);
	}
private:
	func_t m_fun;
};

// 定义没有参数的消息
#define DEF_EVENT(CLS, TYPE) \
	struct CU_API CLS : public Event { DECLARE_RTTI(CLS, Event, TYPE) }

DEF_EVENT(UpdateEvent,	 "UPDA");
DEF_EVENT(BegFrameEvent, "BEGF");
DEF_EVENT(EndFrameEvent, "ENGF");

CU_NS_END