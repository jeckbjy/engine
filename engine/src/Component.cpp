//! Scene
#include "Cute/Component.h"

CUTE_NS_BEGIN

Component::Component()
{

}

Component::~Component()
{

}

void Component::setEnabled(bool enable)
{
    if(m_enabled != enable)
    {
        m_enabled = enable;
        // notify
    }
        
}

CUTE_NS_END
