#pragma once
#include "Cute/EventHandler.h"
#include "Cute/FuncTraits.h"

CUTE_NS_BEGIN

template<class F, class Enable = void>
class FuncHandler : public EventHandler
{
};

// û�ж������
template<class F>
class FuncHandler<F, typename std::enable_if<func_traits<F>::length == 1>::type>
	: EventHandler
{
public:
	FuncHandler(F fun){}
	int handle(PacketEvent* ev)
	{
	}

	F m_fun;
};

// Ŀǰ��֧��ע�ᣬ��֧��ɾ��
class CUTE_CORE_API HandlerRegister
{
public:
	enum Result
	{
		RET_WAIT = -1,
		RET_OK	 = 0,
		RET_FAIL = 1,
	};

	HandlerRegister();
	virtual ~HandlerRegister();

	virtual void setup() = 0;
	// ֧�ֵ�����,0��ʾȫ��֧��
	virtual int  type() = 0;

	template<typename F>
	void add(F fun)
	{
		typedef typename std::remove_pointer<typename func_traits<F>::param0_t>::type TMsg;
		uint msgid = TMsg::MSG_ID;
		EventHandler* handler = new FuncHandler<F>(fun, this);
		regist(msgid, handler);
	}

private:
	void regist(uint msgid, EventHandler* handler);
};

CUTE_NS_END
