//! Components
#include "Cute/Transform.h"
#include "Cute/SceneNode.h"

CUTE_NS_BEGIN

Transform::Transform()
{

}

Transform::~Transform()
{

}

Component* Transform::clone()
{
    Transform* result = new Transform();
    return result;
}

void Transform::translate(const Vector3 &delta, TransformSapce space)
{
    switch (space) {
        case TS_LOCAL:
            m_position += m_rotation.rotate(delta);
            break;
        case TS_PARENT:
            m_position += delta;
            break;
        case TS_WORLD:
        {
            Transform* parent = getParentTransform();
            m_position += (parent != NULL) ? parent->getWorldMatrix().inverse().multiply(delta) : delta;
            break;
        }
        default:
            break;
    }
    
    markDirty();
}

void Transform::rotate(const Quaternion &delta, TransformSapce space)
{
    switch (space) {
        case TS_LOCAL:
            m_rotation = (m_rotation * delta);
            m_rotation.normalize();
            break;
        case TS_PARENT:
            m_rotation = (delta * m_rotation);
            m_rotation.normalize();
            break;
        case TS_WORLD:
        {
            Transform* parent = getParentTransform();
            if(parent)
            {
                m_rotation = m_rotation * getWorldRotation().inverse() * delta * getWorldRotation();
            }
            else
            {
                m_rotation = (delta * m_rotation);
                m_rotation.normalize();
            }
            break;
        }
        default:
            break;
    }
    
    markDirty();
}

void Transform::rotateAround(const Vector3& point, const Quaternion& delta, TransformSapce space)
{
    Vector3 parentSpacePoint;
    Quaternion oldRotation = m_rotation;
    switch (space) {
        case TS_LOCAL:
        {
            parentSpacePoint = getMatrix().multiply(point);
            m_rotation = m_rotation * delta;
            m_rotation.normalize();
            break;
        }
        case TS_PARENT:
        {
            parentSpacePoint = point;
            m_rotation = delta * m_rotation;
            m_rotation.normalize();
            break;
        }
        case TS_WORLD:
        {
            Transform* parent = getParentTransform();
            if(parent != NULL)
            {
                parentSpacePoint = parent->getWorldMatrix().inverse().multiply(point);
                const Quaternion& worldRotation = getWorldRotation();
                m_rotation = m_rotation * worldRotation.inverse() * delta * worldRotation;
            }
            else
            {
                parentSpacePoint = point;
                m_rotation = (delta * m_rotation);
                m_rotation.normalize();
            }
            break;
        }
        default:
            break;
    }
    
    Vector3 oldRelativePos = oldRotation.inverse().rotate(m_position - parentSpacePoint);
    m_position = m_rotation.rotate(oldRelativePos) + parentSpacePoint;
    markDirty();
}

bool Transform::lookAt(const Vector3 &target, const Vector3& up, TransformSapce space)
{
    Vector3 worldSpaceTarget;
    
    switch (space)
    {
        case TS_LOCAL:
            worldSpaceTarget = getWorldMatrix().multiply(target);
            break;
            
        case TS_PARENT:
        {
            Transform* parent = getParentTransform();
            worldSpaceTarget = (parent == NULL) ? target : parent->getWorldMatrix().multiply(target);
            break;
        }
        case TS_WORLD:
            worldSpaceTarget = target;
            break;
    }
    
    Vector3 lookDir = worldSpaceTarget - getWorldPosition();
    // Check if target is very close, in that case can not reliably calculate lookat direction
    if (lookDir == Vector3::ZERO)
        return false;
    Quaternion newRotation;
    // Do nothing if setting look rotation failed
    if (!newRotation.lookRotation(lookDir, up))
        return false;
    
    setWorldRotation(newRotation);
    return true;
}

void Transform::yaw(float angle, TransformSapce space)
{
    rotate(Quaternion(Vector3::UP, angle), space);
}

void Transform::pitch(float angle, TransformSapce space)
{
    rotate(Quaternion(Vector3::RIGHT, angle), space);
}

void Transform::roll(float angle, TransformSapce space)
{
    rotate(Quaternion(Vector3::FORWARD, angle), space);
}

Transform* Transform::getParentTransform() const
{
    if(m_owner->getParent())
        return m_owner->getParent()->getTransform();
    
    return NULL;
}

void Transform::markDirty()
{
    // Precondition:
    // a) whenever a node is marked dirty, all its children are marked dirty as well.
    // b) whenever a node is cleared from being dirty, all its parents must have been
    //    cleared as well.
    // Therefore if we are recursing here to mark this node dirty, and it already was,
    // then all children of this node must also be already dirty, and we don't need to
    // reflag them again.

    // 实际并不需要通知子节点 DIRTY_LOCAL_MATRIX
    m_dirty |= DIRTY_LOCAL_MATRIX;
    // 遍历子节点
    
    if((m_dirty & (~DIRTY_WORLD_MATRIX)) != 0)
        return;

    // 子节点
    m_dirty |= DIRTY_WORLD_MATRIX;
    for(uint i = 0; i < m_owner->getNumChildren(); ++i)
    {
        Transform* node = m_owner->getChild(i)->getTransform();
        node->markDirty();
    }
}

void Transform::setRotation(const Quaternion &rotate)
{
    m_rotation = rotate;
    markDirty();
}

void Transform::setPosition(const Vector3 &pos)
{
    m_position = pos;
    markDirty();
}

void Transform::setScale(const Vector3 &scale)
{
    m_scale = scale;
    markDirty();
}

void Transform::setWorldRotation(const Quaternion &rotation)
{
    Transform* parent = getParentTransform();
    setRotation(parent ? parent->getWorldRotation().inverse() * rotation : rotation);
}

void Transform::setWorldPosition(const Vector3 &pos)
{
    Transform* parent = getParentTransform();
    setPosition(parent ? parent->getWorldMatrix().inverse().multiply(pos) : pos);
}

void Transform::setWorldScale(const Vector3 &scale)
{
    Transform* parent = getParentTransform();
    // is right ??
    if(parent != NULL)
    {
        Matrix3 rotScale;
        parent->getWorldMatrix().getMatrix3(rotScale);
        rotScale.inverse();
        
        Matrix3 scaleMat = Matrix3(Quaternion::IDENTITY, scale);
        scaleMat = rotScale * scaleMat;
        
        Quaternion rotation;
        Vector3 localScale;
        scaleMat.decomposition(rotation, localScale);
        setScale(localScale);
        // or urho3d method:
        // setScale(scale / parent->getWorldScale());
    }
    else
    {
        setScale(scale);
    }
}

const Quaternion& Transform::getRotation() const
{
    return m_rotation;
}

const Vector3& Transform::getPosition() const
{
    return m_position;
}

const Vector3& Transform::getScale() const
{
    return m_scale;
}

const Matrix4& Transform::getMatrix() const
{
    updateLocalTransform();
    return m_localMatrix;
}

const Quaternion& Transform::getWorldRotation() const
{
    updateWorldTransform();
    return m_worldRotation;
}

const Vector3& Transform::getWorldPosition() const
{
    updateWorldTransform();
    return m_worldPosition;
}

const Vector3& Transform::getWorldScale() const
{
    updateWorldTransform();
    return m_worldScale;
}

const Matrix4& Transform::getWorldMatrix() const
{
    updateWorldTransform();
    return m_worldMatrix;
}

void Transform::updateLocalTransform() const
{
    if((m_dirty & DIRTY_LOCAL_MATRIX) != 0)
    {
        m_localMatrix.setTransform(m_position, m_scale, m_rotation);
        m_dirty &= ~DIRTY_LOCAL_MATRIX;
    }
}

void Transform::updateWorldTransform() const
{
    if((m_dirty & DIRTY_WORLD_MATRIX) == 0)
        return;
    
    m_dirty &= ~DIRTY_WORLD_MATRIX;
    
    Transform* parent = getParentTransform();
    if(parent != NULL)
    {
        const Quaternion& protation = parent->getWorldRotation();
        const Vector3&    pscale = parent->getWorldScale();
        
        m_worldRotation = protation * m_rotation;
        m_worldScale    = pscale * m_scale;
        m_worldPosition = protation.rotate(pscale * m_position);
        m_worldPosition+= parent->getWorldPosition();
        m_worldMatrix.setTransform(m_worldPosition, m_worldScale, m_worldRotation);
    }
    else
    {
        m_worldRotation = m_rotation;
        m_worldPosition = m_position;
        m_worldScale    = m_scale;
        m_worldMatrix   = m_localMatrix;
    }
}

CUTE_NS_END
