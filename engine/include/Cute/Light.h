#pragma once
#include "Cute/Component.h"
#include "Cute/Color.h"

CUTE_NS_BEGIN

enum LightType
{
    LIGHT_DIRECTIONAL = 0,
    LIGHT_SPOT,
    LIGHT_POINT,
};

struct BiasDesc
{
    
};

struct CascadeDesc
{
    
};

struct FocusDesc
{
    
};

class CUTE_CORE_API Light : public Component
{
	DECLARE_RTTI(Light, Component, OID_LIGHT);
public:
	Light();
	~Light();
    
    void setType(LightType type);
    void setColor(const Color& color);
    void setTemperature(float value);
    void setRadius(float radius);
    void setLength(float length);
    void setIntensity(float intensity);
    void setBrightness(float brightness);
    void setRange(float range);
    void setFov(float fov);
    void setAspectRatio(float value);
    void setFadeDistance(float distance);
    
    LightType getType() const;
    const Color& getColor() const;
    float getTemperature() const;
    float getRadius() const;
    float getLength() const;
    float getIntensity() const;
    float getBrightness() const;
    float getRange() const;
    float getFov() const;
    float getAspectRatio() const;
    float getFadeDistance() const;

protected:
    LightType   m_type;
    Color       m_color;
    float       m_temperature;
    float       m_radius;
    float       m_length;
    float       m_intensity;
    float       m_brightness;
    float       m_range;
    float       m_fov;
    float       m_aspectRatio;
    float       m_fadeDistance;
    
    float       m_shadowFadeDistance;
    float       m_shadowIntensity;
    float       m_shadowResolution;
    float       m_shadowNearFarRatio;
    float       m_shadowMaxExtrusion;
    
    bool        m_perVertex;
    bool        m_usePhysical;
};

CUTE_NS_END
