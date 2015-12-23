#pragma once
#include "Object.h"
#include "IOOperation.h"

CU_NS_BEGIN
//#define CU_HAS_AIO

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
	IOService*	m_serivce;
	bool		m_attached;
	error_t		m_code;
};

CU_NS_END