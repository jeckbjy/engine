#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Scene : public Object
{
    DECLARE_RTTI(Scene, Object, OID_ANY);
public:
    Scene();
    ~Scene();
    
//    void getComponents(Vector<Component*>& results, const Camera* camera, int type);
//    void getRenderables(Vector<Renderable*>& results, const Camera* camera);
    
protected:
    
};

CUTE_NS_END
