//! Scene
#include "Cute/Entity.h"
#include "Cute/Component.h"
#include "Cute/Stream.h"

CUTE_NS_BEGIN

Entity::Entity()
{
    
}

Entity::~Entity()
{
    // 释放component
//    freeAll(m_components);
    foreach(Component* co, m_components)
    {
        co->release();
    }
}

bool Entity::load(Stream *stream)
{
    return true;
}

void Entity::save(Stream* stream)
{
    
}

void Entity::addComponent(Component *component)
{
    if(component != NULL)
    {
        m_components.push_back(component);
    }
}

void Entity::delComponent(Component *component)
{
    if(component != NULL)
    {
        ComponentList::iterator itor = std::find(m_components.begin(), m_components.end(), component);
        if(itor != m_components.end())
        {
            m_components.erase(itor);
        }
        
        component->release();
    }
}

Component* Entity::getComponent(uint index) const
{
    return m_components[index];
}

Component* Entity::getComponent(const RTTI *rtti) const
{
    foreach(Component* co, m_components)
    {
        if(co->isKindOf(rtti))
            return co;
    }
    
    return NULL;
}

Entity::ComponentArray Entity::getComponents(const RTTI *rtti) const
{
    ComponentArray results;
    foreach(Component* co, m_components)
    {
        if(co->isKindOf(rtti))
        {
            results.push_back(co);
        }
    }
    
    return results;
}

CUTE_NS_END
