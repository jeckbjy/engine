#pragma once
#include "API.h"
#include "Transform.h"
#include "Component.h"

CU_NS_BEGIN

class Transform;
// 必然会持有一个Transform
class CU_API Entity : public Object
{
	DECLARE_RTTI(Entity, Object, "ENTI")

public:
	typedef List<Component> ComponentList;
	typedef ComponentList::iterator iterator;

public:
	Entity();
	Entity(Transform* trans);
	~Entity();

	bool load(Stream* stream);
	void save(Stream* stream);

	void addComponent(Component* component);
	void removeComponent(Component* component);
	Component* getComponent(const Rtti* rtti) const;
	ComponentVec getComponents(const Rtti* rtti) const;

	template<typename T>
	T* getComponent() const
	{
		return (T*)getComponent(T::getStaticRtti());
	}

	template<typename T>
	Vector<T*> getComponents() const;

	Transform* getTransform() const { return getComponent<Transform>(); }

	const String& getName() const { return m_name; }
	void setName(const String& name) { m_name = name; }
	size_t size() const { return m_components.size(); }
	iterator begin() const { return m_components.begin(); }
	iterator end() const { return m_components.end(); }

private:
	String m_name;
	ComponentList m_components;
};

template<typename T>
Vector<T*> Entity::getComponents() const
{
	const Rtti* rtti = T::getRttiInstance();
	Vector<T*> result;
	for (ComponentList::iterator itor = m_components.begin(); itor != m_components.end(); ++itor)
	{
		if (itor->isKindOf(rtti))
		{
			result.push_back(*itor);
		}
	}
	return result;
}

CU_NS_END