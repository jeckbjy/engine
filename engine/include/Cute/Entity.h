#pragma once
#include "Cute/Object.h"
#include "Cute/List.h"
#include "Cute/Component.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Entity : public TObject<Entity>
{
public:
    typedef Vector<Component*> ComponentList;
    typedef Vector<Component*> ComponentArray;
    Entity();
    ~Entity();
    
    bool load(Stream* stream);
    void save(Stream* stream);
    
    void addComponent(Component* component);
    void delComponent(Component* component);
    
    Component*      getComponent(uint index) const;
    Component*      getComponent(const RTTI* rtti) const;
    ComponentArray  getComponents(const RTTI* rtti) const;
    
    template<typename T>
    T*              createComponent();
    
    template<typename T>
    T*              getComponent();
    template<typename T>
    Vector<T*>      getComponents();
    
    template<typename T>
    T*              getOrCreateComponent();
    
    uint            getNumConponents() const;
    
protected:
    ComponentList m_components;
};

//
// inline
//
inline uint Entity::getNumConponents() const
{
    return m_components.size();
}

template<typename T>
T* Entity::getComponent()
{
    return (T*)getComponent(T::getStaticRTTI());
}

template<typename T>
Vector<T*> Entity::getComponents()
{
    const RTTI* rtti = T::getStaticRTTI();
    Vector<T*> result;
    foreach(const Component* co, m_components)
    {
        if(co->isKindOf(rtti))
        {
            result.push_back((T*)co);
        }
    }
    return result;
}

template<typename T>
T* Entity::createComponent()
{
    T* co = new T();
    addComponent(co);
    return co;
}

template<typename  T>
T* Entity::getOrCreateComponent()
{
    T* co = getComponent<T>();
    if(co != NULL)
        return co;
    
    return createComponent<T>();
}

CUTE_NS_END
