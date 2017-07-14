#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Technique : public Object
{
    DECLARE_RTTI(Technique, Object, OID_ANY)
public:
    Technique();
    ~Technique();
};

class CUTE_CORE_API Shader : public Object
{
    DECLARE_RTTI(Shader, Object, OID_ANY)
public:
//    typedef Vector<>
    Shader();
    ~Shader();
    
private:
    
};

CUTE_NS_END
