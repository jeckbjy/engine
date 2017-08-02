#pragma once
#include "Cute/Component.h"

CUTE_NS_BEGIN

//class CUTE_CORE_API RenderContext
//{
//public:
//    virtual void draw(Mesh* mesh);
//    
//private:
//    Camera* m_camera;
//};

// LOD support: LODGroup
class CUTE_CORE_API Renderable : public Component
{
public:
    Renderable();
    ~Renderable();

    virtual void draw(Camera* camera) = 0;
    virtual void drawDebug() = 0;
    
    virtual Mesh*       getMesh() const { return NULL; }
    virtual Material*   getMaterial(uint index) const { return NULL; }
};

CUTE_NS_END
