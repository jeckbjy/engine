#pragma once
#include "Cute/Component.h"
#include "Cute/Matrix4.h"
#include "Cute/Frustum.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Camera : public Component
{
	DECLARE_RTTI(Camera, Object, OID_CAMERA)
public:
	Camera();
	~Camera();

protected:
    mutable Matrix4 m_view;
    mutable Matrix4 m_proj;
    mutable Frustum m_frustum;
    mutable bool    m_viewDirty : 1;
    mutable bool    m_projDirty : 1;
    mutable bool    m_frustumDirty : 1;
    bool            m_orthographic;
    
    mutable float   m_projNearClip;
    mutable float   m_projFarClip;
    
    float m_nearClip;
    float m_farClip;
    float m_fov;
    float m_orthoSize;
    float m_aspectRatio;

    
};

CUTE_NS_END
