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
	typedef Function<F> func_t;
	func_t m_fun;
	THandlerBase(F fun, void* owner = 0) :m_fun(fun, owner){}
};

template<typename F, typename >
struct HandleTraits
{

};

template<typename F>
struct THandler : public THandlerBase<F>
{
	THandler(F fun, void* owner = 0) : THandlerBase<F>(fun, owner){}
	int process(IPacket* pkg, Session* sess)
	{
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