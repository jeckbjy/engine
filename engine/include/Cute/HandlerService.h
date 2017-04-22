#pragma once
#include "Cute/EventHandler.h"

CUTE_NS_BEGIN

// ��������Handler,����HANDLER_MAX��������,���ڵķ���Map
class CUTE_CORE_API HandlerService
{
public:
	static HandlerService& instance();

	HandlerService();
	~HandlerService();

	EventHandler* find(uint32 msgid) const;
	bool has(uint32 msgid) const;
	void regist(uint32 msgid, EventHandler* handler);
	void remove(uint32 msgid);

private:
	enum
	{
		HANDLER_MAX = 2048,
	};

	typedef std::vector<EventHandler*>		HandlerVec;
	typedef std::map<uint32, EventHandler*>	HandlerMap;
	HandlerVec	m_handlerVec;
	HandlerMap	m_handlerMap;
};

CUTE_NS_END
