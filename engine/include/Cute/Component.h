#pragma once
#include "Cute/Object.h"
#include "Cute/List.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Component : public TObject<Component, OID_COMPONENT>, public ListHook
{
public:
	Component();
	~Component();
    
    virtual Component* clone() = 0;
    
    bool isEnabled() const;
    void setEnabled(bool enable);

protected:
    SceneNode*  m_owner;
    bool        m_enabled;
};

//
//
//
inline bool Component::isEnabled() const
{
    return m_enabled;
}

CUTE_NS_END
