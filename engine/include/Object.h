#pragma once
#include "Ref.h"
#include "Event.h"

CU_NS_BEGIN

// 系统已知类型定义
enum ObjectType
{
	TYPE_UNKNOWN,

	TYPE_OBJECT,

	// render 
	TYPE_COMPONENT,
	TYPE_CAMERA,
	TYPE_LIGHT,
	TYPE_ANIMATOR,
	TYPE_NODE,
};

class CU_API Object : public RefCounted
{
	DECLARE_RTTI(Object, RootRtti, TYPE_OBJECT);
public:
	virtual ~Object();

	uint32_t getObjectType() const { return getRtti()->getType(); }

	bool isKindOf(const Rtti* rtti) const { return getRtti()->isKindOf(rtti); }

	virtual void addAttributes() const { }
	const AttributeList& getAttributes() const;

	void addListener(EventHandler* handler);
	void removeListener(EventHandler* handler);
	void removeListener(uint32_t type, Object* sender = NULL);

public:
	template<typename T>
	inline bool isKindOf() const
	{
		return getRtti()->isKindOf(T::getStaticRtti());
	}

	template<typename T> 
	inline T* cast() const
	{
		return isKindOf<T>() ? (T*)(this) : NULL;
	}

	template<typename F>
	inline void addListener(F fun, void* udata = 0, Object* sender = NULL) 
	{
		typedef func_traits<F>::class_t  class_t;
		typedef std::remove_pointer<func_traits<F>::param0_t>::type event_t;
		const Rtti* rtti = event_t::getStaticRtti();
		EventHandler* handler = new TEventHandler<F>(fun, rtti->getType(), (class_t*)this, sender, udata);
		addListener(handler);
	}

	template<typename TEVENT>
	inline void removeListener(Object* sender = NULL)
	{
		removeListener(TEVENT::getStaticRtti()->getType(), sender);
	}

private:
	typedef List<EventHandler, 1> HandlerList;
	HandlerList m_handlers;
};

CU_NS_END
