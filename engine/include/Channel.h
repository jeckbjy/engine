#pragma once
#include "Object.h"
#include "IOOperation.h"

CU_NS_BEGIN
//#define CU_HAS_AIO

// Ҫ���ⲿ���ñ���ʹ�����ü�������ΪIOOperation��ʹ����
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
	// socket����״̬
	enum State
	{
		S_UNKNOWN,		// ��δ��ʼ����
		S_CONNECTING,	// �������ӷ�������
		S_DISCONNECT,	// δ����
		S_ESTABLISH,	// �Ѿ���������
	};

	IOService*	m_serivce;
	error_t		m_code;
};

CU_NS_END