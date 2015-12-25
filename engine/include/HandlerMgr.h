#pragma once
#include "API.h"
#include "Function.h"
#include "FuncTraits.h"

CU_NS_BEGIN

enum Error
{
	ERR_OK,		// 成功
	ERR_WAIT,	// 阻塞了，未完成,需要重新投递
	ERR_FAIL,	// 失败
	ERR_DECODE,
	ERR_PARAM,	// 参数错误
};

struct IPacket;
class Session;
struct IHandler
{
	virtual ~IHandler(){}
	virtual int process(IPacket* pkg, Session* sess) = 0;
};

// traits func
template<typename F>
struct THandlerBase : public IHandler
{
	typedef typename std::remove_pointer<typename func_traits<F>::param0_t>::type msg_t;
	typedef typename std::remove_pointer<typename func_traits<F>::param1_t>::type arg_t;
	typedef Function<F> func_t;
	func_t m_fun;
	THandlerBase(F fun, void* owner = 0) :m_fun(fun, owner){}
};

template<typename T>
struct SessionConvertor
{
	inline static T* convert(Session* sess);
};

template<>
struct SessionConvertor<Session>
{
	inline static Session* convert(Session* sess) { return sess; }
};

// 2个参数时会自动萃取长度
template<class F, class Msg, class Arg>
struct Invoker
{
	static int call(F fun, Msg* msg, Session* sess)
	{
		Arg* arg = SessionConvertor<Arg>::convert(sess);
		if (arg == 0)
			return ERR_PARAM;
		return fun(msg, arg);
	}
};

// 1个参数时
template<class F, class Msg>
struct Invoker<F, Msg, void>
{
	static void call(F& fun, Msg* msg, Session* sess)
	{
		return fun(msg);
	}
};

template<typename F>
struct THandler : public THandlerBase<F>
{
	THandler(F fun, void* owner = 0) : THandlerBase<F>(fun, owner){}
	int process(IPacket* pkg, Session* sess)
	{
		Invoker<func_t, msg_t, arg_t>::call(m_fun, (msg_t*)pkg, sess);
		return 0;
	}
};

class CU_API HandlerMgr
{
public:
	static HandlerMgr* instance();
	HandlerMgr();
	~HandlerMgr();

	IHandler* find(uint msgid);
	void add_handler(uint msgid, IHandler* handler);

	template<typename F, template<class> class H>
	void add_func(F fun, void* owner)
	{
		typedef typename std::remove_pointer<typename func_traits<F>::param0_t>::type TMsg;
		uint msgid = TMsg::MSG_ID;
		IHandler* handler = new H<F>(fun, owner);
		add_handler(msgid, handler);
	}

	template<typename F>
	void add(F fun, void* owner)
	{
		add_func<F, THandler>(fun, owner);
	}

protected:
	typedef std::vector<IHandler*> HandlerVec;
	HandlerVec m_handlers;
};

CU_NS_END