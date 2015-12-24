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
	THandlerBase(F fun, void* owner) :m_fun(fun, owner){}
};

template<typename F>
struct THandler : public THandlerBase<F>
{
	int process(IPacket* pkg, Session* sess)
	{
		return 0;
	}
};

// example//template<typename F>
//struct TPlayerHandler : public THandlerBase<F>
//{
//	int process(IPacket* pkg, Session* sess)
//	{
//		// check player 等
//		return 0;
//	}
//};

class CU_API HandlerMgr
{
public:
	HandlerMgr();
	~HandlerMgr();

	IHandler* find(uint msgid);
	void add_handler(uint msgid, IHandler* handler);

	template<typename F, template<class> class H>
	void add(F fun)
	{
		typedef typename std::remove_pointer<typename func_traits<F>::param0_t>::type TMsg;
		uint msgid = TMsg::PACK_ID;
		IHandler* handler = new H<F>(fun, this);
		add_handler(msgid, handler);
	}

protected:
	typedef std::vector<IHandler*> HandlerVec;

};

CU_NS_END