#include "Entity.h"

CU_NS_BEGIN

Entity::Entity()
{
	// 默认会自动添加一个Transform
	Transform* trans = new Transform();
	addComponent(trans);
}

Entity::Entity(Transform* trans)
{
	addComponent(trans);
}

Entity::~Entity()
{
	for (ComponentList::iterator itor = m_components.begin(); itor != m_components.end(); ++itor)
	{
		itor->release();
	}
	m_components.clear();
}

bool Entity::load(Stream* stream)
{
	uint32 magic;
	uint comp_size = stream->readVariant();
	for (uint i = 0; i < comp_size; ++i)
	{
		stream->read(magic);
		// 根据类型创建
		Component* component = Rtti::create<Component>(magic);
		if (!component)
			continue;
		component->load(stream);
	}
	return true;
}

void Entity::save(Stream* stream)
{
	uint32 magic;
	Component* component;
	stream->writeVariant(m_components.size());
	for (iterator itor = m_components.begin(); itor != m_components.end(); ++itor)
	{
		component = *itor;
		magic = component->getRtti()->getFourCC();
		stream->write(magic);
		component->save(stream);
	}
}

void Entity::addComponent(Component* component)
{
	m_components.push(component);
	component->attach(this);
}

void Entity::removeComponent(Component* component)
{
	assert(!component->isKindOf<Transform>());
	component->detach();
	m_components.erase(component);
}

Component* Entity::getComponent(const Rtti* rtti) const
{
	for (ComponentList::iterator itor = m_components.begin(); itor != m_components.end(); ++itor)
	{
		if (itor->isKindOf(rtti))
			return *itor;
	}

	return NULL;
}

ComponentVec Entity::getComponents(const Rtti* rtti) const
{
	ComponentVec result;
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

