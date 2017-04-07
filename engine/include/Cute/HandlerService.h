#pragma once
#include "Cute/Handler.h"

CUTE_NS_BEGIN

// HandlerRoute??
class CUTE_CORE_API HandlerService
{
public:
	static HandlerService& get();

	HandlerService();
	~HandlerService();

	Handler* find(uint32 msgid) const;
	bool has(uint32 msgid) const;
	void regist(uint32 msgid, Handler* handler);
	void remove(uint32 msgid);

private:
	enum
	{
		HANDLER_MAX = 2048,
	};

	typedef std::vector<Handler*>		HandlerVec;
	typedef std::map<uint32, Handler*>	HandlerMap;
	HandlerVec	m_handlerVec;
	HandlerMap	m_handlerMap;
};

CUTE_NS_END
