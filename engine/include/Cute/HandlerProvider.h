#pragma once
#include "Cute/Handler.h"
#include "Cute/FuncTraits.h"

CUTE_NS_BEGIN

template<class F, class Enable = void>
class FuncHandler : public Handler
{
};

// û�ж������
template<class F>
class FuncHandler<F, typename std::enable_if<func_traits<F>::length == 1>::type>
	: Handler
{
public:
	FuncHandler(F fun){}
	void handle(PacketEvent* ev)
	{
		// �Ƚ��룿��
	}

	F m_fun;
};

// ��һ���������


/*
usage:
class GameHandlerProvider : public HandlerProvider
{
public:
	void setup()
	{
		regist(&GameHandlerProvider::onLogin);
		regist(&GameHandlerProvider::onLogout);
	}

	void onLogin(LoginReq* req, User*)
	{
	}

	void onLogout(LogoutReq* req, User*)
	{
	}
}
*/
class HandlerProvider
{
public:
	enum Result
	{
		RET_WAIT = -1,
		RET_OK,
		RET_FAIL,
	};

	HandlerProvider();
	virtual ~HandlerProvider();

	virtual void setup() = 0;
	virtual void cleanup();

	template<typename F>
	void regist(F fun)
	{
		typedef typename std::remove_pointer<typename func_traits<F>::param0_t>::type TMsg;
		uint msgid = TMsg::MSG_ID;
		Handler* handler = new Handler(fun, this);
		regist(msgid, handler);
	}

private:
	void regist(uint msgid, Handler* handler);

private:
	Handler* m_head;
};

// HandlerAdapter
//class GameHandler : public HandlerProvider
//{
//public:
//	void setup();
//
//	void onLogin(LoginReq* req, User* user);
//};

CUTE_NS_END
