#pragma once
#include "Cute/Component.h"
#include "Cute/Matrix4.h"
#include "Cute/Frustum.h"
#include "Cute/Ray.h"

CUTE_NS_BEGIN

// TODO:study clipPlane, reflectionPlane, customProjection
class CUTE_CORE_API Camera : public Component
{
	DECLARE_RTTI(Camera, Component, OID_CAMERA)
public:
	Camera();
	virtual ~Camera();
    
    virtual void onTransformChanged() OVERRIDE;
    
    void setFov(float fov);
    void setFarClip(float farClip);
    void setNearClip(float nearClip);
    void setOrthoSize(float orthoSize);
    void setOrthoSize(const Vector2& orthoSize);
    void setAspectRatio(float aspect);
    
    void setZoom(float zoom);
    void setLodBias(float bias);
    void setViewMask(uint mask);
    void setViewOverrideFlag(uint flags);
//    void setFillMode(FILL_MODE mode);
    
    void setOrthographic(bool enable);
    void setAutoAspectRatio(bool enable);
    void setProjectionOffset(const Vector2& offset);
    void setUseReflection(bool enable);
    void setReflectionPlane(const Plane& plane);
    void setUseClipping(bool enable);
    void setClipPlane(const Plane& plane);
    
    void setProjection(const Matrix4& proj);
    
    float getFov() const;
    float getFarClip() const;
    float getNearClip() const;
    float getOrthoSize() const;
    float getAspectRatio() const;
    float getZoom() const;
    float getLodBias() const;
    
    uint  getViewMask() const;
    uint  getViewOverrideFlags() const;
//    FILL_MODE getFillMode() const;
    
    bool  isOrthographic() const;
    bool  isAutoAspectRatio() const;
    
    float getDistance(const Vector3& worldPos) const;
    float getDistanceSquared(const Vector3& worldPos) const;
    float getLodDistance(float distance, float scale, float bias) const;
    
    const Frustum&  getFrustum() const;
    const Matrix4&  getProjection() const;
    const Matrix4&  getView() const;
    
    void            getFrustumSize(Vector3& near, Vector3& far) const;
    float           getHalfViewSize() const;
    
    Ray             getScreenRay(float x, float y) const;
    
protected:
    void updateProjection() const;
    
protected:
    enum
    {
        DIRTY_PROJ          = 0x01,
        DIRTY_VIEW          = 0x02,
        DIRTY_FRUSTUM       = 0x04,
        DIRTY_PROJ_FRUSTUM  = DIRTY_PROJ | DIRTY_FRUSTUM,
        DIRTY_ALL           = DIRTY_PROJ | DIRTY_FRUSTUM | DIRTY_VIEW
    };
    
    mutable Matrix4 m_view;
    mutable Matrix4 m_proj;
    mutable Frustum m_frustum;
    mutable uint8   m_flags;
    
    bool    m_autoAspectRatio : 1;
    bool    m_ortho : 1;
    
    float   m_fov;
    float   m_farClip;
    float   m_nearClip;
    float   m_orthoSize;
    float   m_aspectRatio;

    float   m_zoom;
    float   m_lodBias;
    
    Vector2 m_projOffset;
    
    uint    m_viewMask;
    uint    m_viewOverrideFlags;
};

CUTE_NS_END
