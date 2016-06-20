#pragma once
#include "List.h"
#include "Stream.h"
#include "Object.h"
#include "Animatable.h"

CU_NS_BEGIN

class CU_API Component : public Object, public Animatable, public ListNode<Component>
{
	friend class Entity;
	DECLARE_RTTI(Component, Object, OBJ_ID_COMPONENT);
public:
	Component();
	virtual ~Component();

	virtual void load(Stream* stream){}
	virtual void save(Stream* stream){}

	virtual void attach(Entity* entity);
	virtual void detach();

	const String& getName() const { return m_name; }
	Entity* getEntity() { return m_owner; }

protected:
	Entity*	m_owner;
	String	m_name;
};

typedef Vector<Component*> ComponentVec;

CU_NS_END