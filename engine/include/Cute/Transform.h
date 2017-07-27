#pragma once
#include "Cute/Component.h"
#include "Cute/Matrix4.h"
#include "Cute/Vector3.h"
#include "Cute/Quaternion.h"

CUTE_NS_BEGIN

enum TransformSapce
{
    TS_LOCAL = 0,
    TS_PARENT,
    TS_WORLD
};

class CUTE_CORE_API Transform : public Component
{
	DECLARE_RTTI(Transform, Component, OID_TRANSFORM)
public:
	Transform();
	~Transform();

    virtual Component* clone();
    
    void translate(const Vector3& delta, TransformSapce space = TS_LOCAL);
    void rotate(const Quaternion& delta, TransformSapce space = TS_LOCAL);
    void rotateAround(const Vector3& point, const Quaternion& delta, TransformSapce space = TS_LOCAL);
    void pitch(float angle, TransformSapce space = TS_LOCAL);
    void yaw(float angle, TransformSapce space = TS_LOCAL);
    void roll(float angle, TransformSapce space = TS_LOCAL);
    bool lookAt(const Vector3& target, const Vector3& up = Vector3::UNIT_Y, TransformSapce space = TS_WORLD);
    // in parent space
    void scale(const Vector3& scale);
    
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
