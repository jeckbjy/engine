//! Components
#include "Cute/Light.h"
#include "Cute/Math.h"

CUTE_NS_BEGIN

Light::Light()
{

}

Light::~Light()
{

}

void Light::setType(LightType type)
{
    m_type = type;
    // TODO:notify changed
}

void Light::setColor(const Color &color)
{
    m_color.set(color.r, color.g, color.b);
}

void Light::setTemperature(float value)
{
    m_temperature = Math::clamp(value, 1000.0f, 10000.0f);
}

void Light::setRadius(float radius)
{
    m_radius = radius;
}

void Light::setLength(float length)
{
    m_length = length;
}

void Light::setIntensity(float intensity)
{
    m_intensity = intensity;
}

void Light::setBrightness(float brightness)
{
    m_brightness = brightness;
}

void Light::setRange(float range)
{
    m_range = Math::abs(range);
}

void Light::setFov(float fov)
{
    m_fov = Math::clamp(fov, 0.0f, Math::MAX_FOV);
    // mark dirty
}

void Light::setAspectRatio(float value)
{
    m_aspectRatio = Math::max(value, Math::EPSILON);
}

void Light::setFadeDistance(float distance)
{
    m_fadeDistance = Math::abs(distance);
}

LightType Light::getType() const
{
    return m_type;
}

const Color& Light::getColor() const
{
    return m_color;
}

float Light::getTemperature() const
{
    return m_temperature;
}

float Light::getRadius() const
{
    return m_radius;
}

float Light::getLength() const
{
    return m_length;
}

float Light::getIntensity() const
{
    return m_intensity;
}

float Light::getBrightness() const
{
    return m_brightness;
}

float Light::getRange() const
{
    return m_range;
}

float Light::getFov() const
{
    return m_fov;
}

float Light::getAspectRatio() const
{
    return m_aspectRatio;
}

float Light::getFadeDistance() const
{
    return m_fadeDistance;
}

CUTE_NS_END
