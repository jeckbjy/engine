#pragma once
#include "List.h"
#include "Stream.h"
#include "Object.h"
#include "Animatable.h"

CU_NS_BEGIN

class Entity;
class CU_API Component : public Object, public Animatable, public ListNode<Component>
{
	friend class Entity;
	DECLARE_RTTI(Component, Object, TYPE_COMPONENT);
public:
	Component();
	virtual ~Component();

	virtual void draw(){}
	virtual void load(Stream* stream){}
	virtual void save(Stream* stream){}

	const String& getName() const { return m_name; }
	Entity* getEntity() { return m_owner; }

protected:
	Entity*	m_owner;
	String	m_name;
};

typedef Vector<Component*> ComponentVec;

CU_NS_END