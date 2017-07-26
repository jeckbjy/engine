#pragma once
#include "Cute/Component.h"
#include "Cute/Quaternion.h"
#include "Cute/Vector3.h"
#include "Cute/Matrix4.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Transform : public Component
{
	DECLARE_RTTI(Transform, Component, OID_TRANSFORM)
public:
	Transform();
	~Transform();

    virtual Component* clone();
    
    // local space
    void setRotation(const Quaternion& rotate);
    void setPosition(const Vector3& pos);
    void setScale(const Vector3& scale);
    
    void setWorldRotation(const Quaternion& rotate);
    void setWorldPosition(const Vector3& pos);
    void setWorldScale(const Vector3& scale);
    
    const Quaternion&   getRotation() const;
    const Vector3&      getPosition() const;
    const Vector3&      getScale()    const;
    const Matrix4&      getMatrix()   const;
    
    const Quaternion&   getWorldRotation() const;
    const Vector3&      getWorldPosition() const;
    const Vector3&      getWorldScale()    const;
    const Matrix4&      getWorldMatrix()   const;
    
private:
    Transform*          getParentTransform() const;
    void                updateLocalTransform() const;
    void                updateWorldTransform() const;
    void                markDirty();
    
protected:
    enum DirtyFlag
    {
        DIRTY_NONE          = 0,
        DIRTY_LOCAL_MATRIX  = 0x01,
        DIRTY_WORLD_MATRIX  = 0x02,
    };
    
    Quaternion          m_rotation;
    Vector3             m_position;
    Vector3             m_scale;
    
    mutable Quaternion  m_worldRotation;
    mutable Vector3     m_worldPosition;
    mutable Vector3     m_worldScale;
    
    mutable Matrix4     m_localMatrix;
    mutable Matrix4     m_worldMatrix;
    mutable uint8       m_dirty;
};

CUTE_NS_END
