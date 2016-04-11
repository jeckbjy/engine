#pragma once
#include "Object.h"
#include "IOOperation.h"

CU_NS_BEGIN
//#define CU_HAS_AIO

// 要求外部引用必须使用引用计数，因为IOOperation中使用了
class IOService;
class CU_API Channel : public Object
{
	DECLARE_RTTI(Channel, Object, "CHAN");

public:
	Channel(IOService* serivce = NULL);
	virtual ~Channel();
	virtual handle_t handle() const = 0;
	virtual void perform(IOOperation* op) = 0;

	void attach();
	void detach();

	void setService(IOService* service){ m_serivce = service; }
	error_t getLastCode() const { return m_code; }

protected:
	// socket连接状态
	enum State
	{
		S_UNKNOWN,		// 尚未初始化过
		S_CONNECTING,	// 正在连接服务器中
		S_DISCONNECT,	// 未连接
		S_ESTABLISH,	// 已经建立连接
	};

	IOService*	m_serivce;
	error_t		m_code;
};

CU_NS_END