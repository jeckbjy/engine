//! Scene/Component
#include "Cute/Camera.h"
#include "Cute/SceneNode.h"
#include "Cute/Transform.h"

CUTE_NS_BEGIN

Camera::Camera()
{

}

Camera::~Camera()
{

}

void Camera::onTransformChanged()
{
    
}

void Camera::setFov(float fov)
{
    m_fov = Math::clamp(fov, 0.0f, Math::MAX_FOV);
    CUTE_SET_MASK(m_flags, DIRTY_PROJ_FRUSTUM);
}

void Camera::setFarClip(float farClip)
{
    m_farClip = Math::max(farClip, Math::MIN_NEARCLIP);
    CUTE_SET_MASK(m_flags, DIRTY_PROJ_FRUSTUM);
}

void Camera::setNearClip(float nearClip)
{
    m_nearClip = Math::max(nearClip, Math::MIN_NEARCLIP);
    CUTE_SET_MASK(m_flags, DIRTY_PROJ_FRUSTUM);
}

void Camera::setOrthoSize(float orthoSize)
{
    m_orthoSize = orthoSize;
    m_aspectRatio = 1.0f;
    CUTE_SET_MASK(m_flags, DIRTY_PROJ_FRUSTUM);
}

void Camera::setOrthoSize(const Vector2 &orthoSize)
{
    m_autoAspectRatio = false;
    m_orthoSize = orthoSize.y;
    m_aspectRatio = orthoSize.x / orthoSize.y;
    CUTE_SET_MASK(m_flags, DIRTY_PROJ_FRUSTUM);
}

void Camera::setAspectRatio(float aspect)
{
    
}

void Camera::setZoom(float zoom)
{
    m_zoom = Math::max(zoom, Math::EPSILON);
    CUTE_SET_MASK(m_flags, DIRTY_PROJ_FRUSTUM);
}

void Camera::setLodBias(float bias)
{
    m_lodBias = Math::max(bias, Math::EPSILON);
}

void Camera::setOrthographic(bool enable)
{
    if(m_ortho != enable)
    {
        m_ortho = enable;
        CUTE_SET_MASK(m_flags, DIRTY_PROJ_FRUSTUM);
    }
}

float Camera::getFov() const
{
    return m_fov;
}

float Camera::getFarClip() const
{
    return m_farClip;
}

float Camera::getNearClip() const
{
    return m_ortho ? 0 : m_nearClip;
}

float Camera::getOrthoSize() const
{
    return m_orthoSize;
}

float Camera::getAspectRatio() const
{
    return m_aspectRatio;
}

float Camera::getZoom() const
{
    return m_zoom;
}

float Camera::getLodBias() const
{
    return m_lodBias;
}

uint Camera::getViewMask() const
{
    return m_viewMask;
}

uint Camera::getViewOverrideFlags() const
{
    return m_viewOverrideFlags;
}

bool Camera::isOrthographic() const
{
    return m_ortho;
}

bool Camera::isAutoAspectRatio() const
{
    return m_autoAspectRatio;
}

float Camera::getDistance(const Vector3 &worldPos) const
{
    if(!m_ortho)
    {
        const Vector3& cameraPos = m_owner ? m_owner->getTransform()->getWorldPosition() : Vector3::ZERO;
        return (worldPos - cameraPos).length();
    }
    else
    {
        return Math::abs(getView().multiply(worldPos).z);
    }
}

float Camera::getDistanceSquared(const Vector3 &worldPos) const
{
    if(!m_ortho)
    {
        const Vector3& cameraPos = m_owner ? m_owner->getTransform()->getWorldPosition() : Vector3::ZERO;
        return (worldPos - cameraPos).squared();
    }
    else
    {
        float distance = Math::abs(getView().multiply(worldPos).z);
        return distance * distance;
    }
}

float Camera::getLodDistance(float distance, float scale, float bias) const
{
    float d = Math::max(m_lodBias * bias * scale * m_zoom, Math::EPSILON);
    if (!m_ortho)
        return distance / d;
    else
        
        return m_orthoSize / d;
}

const Frustum& Camera::getFrustum() const
{
    if(CUTE_HAS_MASK(m_flags, DIRTY_PROJ))
        updateProjection();
    
    if(CUTE_HAS_MASK(m_flags, DIRTY_FRUSTUM))
    {
        // update frustum
//        if(!m_ortho)
//            m_frustum.
    }
    return m_frustum;
}

const Matrix4& Camera::getProjection() const
{
    if(CUTE_HAS_MASK(m_flags, DIRTY_PROJ))
        updateProjection();
    
    return m_proj;
}

const Matrix4& Camera::getView() const
{
    if(CUTE_HAS_MASK(m_flags, DIRTY_VIEW))
    {
        CUTE_RESET_MASK(m_flags, DIRTY_VIEW);
//        m_view = 
    }
    return m_view;
}

void Camera::updateProjection() const
{
    CUTE_RESET_MASK(m_flags, DIRTY_PROJ);
    if(m_ortho)
    {
        float h = (1.0f / (m_orthoSize * 0.5f)) * m_zoom;
        float w = h / m_aspectRatio;
        float q = 1.0f / m_farClip;
        float r = 0.0f;
        float offx = m_projOffset.x * 2.0f;
        float offy = m_projOffset.y * 2.0f;
        
        m_proj.set(
                   w,    0.0f, 0.0f, offx,
                   0.0f, h,    0.0f, offy,
                   0.0f, 0.0f, q,    r,
                   0.0f, 0.0f, 0.0f, 1.0f);
    }
    else
    {
        float h = (1.0f / tanf(m_fov * Math::DEG2RAD * 0.5f)) * m_zoom;
        float w = h / m_aspectRatio;
        float q = m_farClip / (m_farClip - m_nearClip);
        float r = -q * m_nearClip;
        float offx = m_projOffset.x * 2.0f;
        float offy = m_projOffset.y * 2.0f;
        
        m_proj.set(
                   w, 0, offx, 0,
                   0, h, offy, 0,
                   0, 0, q, r,
                   0, 0, 1.0f, 0);
    }
}

CUTE_NS_END
