#pragma once
#include "Cute/Component.h"

CUTE_NS_BEGIN

enum ScreenSpace
{
    SCREEN_SPACE_OVERLAY,
    SCREEN_SPACE_CAMERA,
    SCREEN_SPACE_WORLD,
};

// draw ui Renderable
class CUTE_CORE_API Canvas : public Component
{
public:
    Canvas();
    ~Canvas();
    
    Camera* getCamera() const;
    void    setCamera(Camera* camera);
    
    float   getDistance() const;
    void    setDistance(float value);
    
    bool    isPixelPerfect() const;
    void    setPixelPerfect(bool value);
    
private:
    ScreenSpace m_space;
    Camera*     m_camera;
    float       m_distance;
    bool        m_pixelPerfect;
};

//
// inline
//
inline Camera* Canvas::getCamera() const
{
    return m_camera;
}

inline void Canvas::setCamera(Camera *camera)
{
    m_camera = camera;
}

inline float Canvas::getDistance() const
{
    return m_distance;
}

inline void Canvas::setDistance(float value)
{
    m_distance = value;
}

inline bool Canvas::isPixelPerfect() const
{
    return m_pixelPerfect;
}

inline void Canvas::setPixelPerfect(bool value)
{
    m_pixelPerfect = value;
}

CUTE_NS_END
