#include "Transform.h"
#include "Entity.h"

CU_NS_BEGIN

static const int DIRTY_ROTATION = 0x01;
static const int DIRTY_POSITION = 0x02;
static const int DIRTY_SCALE = 0x04;
static const int DIRTY_RTS = 0x07;
static const int DIRTY_LOCAL = 0x08;
static const int DIRTY_WORLD = 0x10;

Transform::Transform()
{

}

Transform::Transform(const Quaternion& rotation, const Vector3& position, const Vector3& scale, TransformSpace space /* = TS_LOCAL */)
{

}

Transform::~Transform()
{

}

Entity* Transform::findEntity(const String& name) const
{
	if (name.empty() || name == m_owner->getName())
		return m_owner;

	// 递归遍历所有子节点
	return NULL;
}

Entity* Transform::findEntityByPath(const String& path) const
{

	return NULL;
}

Transform* Transform::getChild(const String& name, bool recursive /* = false */) const
{
	return NULL;
}

void Transform::translate(const Vector3& position, TransformSpace space /* = TS_LOCAL */)
{

}

void Transform::rotate(const Quaternion& rotation, TransformSpace space)
{

}

void Transform::scale(const Vector3& s, TransformSpace space /* = TS_LOCAL */)
{

}

void Transform::setRotation(const Quaternion& r)
{

}

void Transform::setPosition(const Vector3& t)
{

}

void Transform::setScale(const Vector3& s)
{

}

void Transform::setLocalRotation(const Quaternion& r)
{
	m_localRotation = r;
	m_flags[DIRTY_ROTATION] = true;
}

void Transform::setLocalPosition(const Vector3& t)
{
	m_localPosition = t;
	m_flags[DIRTY_POSITION] = true;
}

void Transform::setLocalScale(const Vector3& s)
{
	m_localScale = s;
	m_flags[DIRTY_SCALE] = true;
}

void Transform::setWorldRotation(const Quaternion& r)
{
	setLocalRotation(m_parent ? m_parent->getWorldRotation().inverse() * r : r);
}

void Transform::setWorldPosition(const Vector3& t)
{
	//setLocalPosition(parent ? parent->getWorldMatrix())
}

void Transform::setWorldScale(const Vector3& s)
{

}

const Quaternion& Transform::getLocalRotation() const
{
	return m_localRotation;
}

const Vector3& Transform::getLocalPosition() const
{
	return m_localPosition;
}

const Vector3& Transform::getLocalScale() const
{
	return m_localScale;
}

const Matrix4& Transform::getLocalMatrix() const
{
	// 处理
	if (m_flags[DIRTY_RTS])
	{
		m_localMatrix.setTransform(m_localPosition, m_localScale, m_localRotation);
		//m_flags.set();
	}

	return m_localMatrix;
}

const Quaternion& Transform::getWorldRotation() const
{
	return m_worldRotation;
}

const Vector3& Transform::getWorldPosition() const
{
	return m_worldPosition;
}

const Vector3& Transform::getWorldScale() const
{
	return m_worldScale;
}

const Matrix4& Transform::getWorldMatrix() const
{
	if (m_flags[DIRTY_WORLD])
	{
		m_flags[DIRTY_WORLD] = false;
		m_worldMatrix = m_parent ? m_parent->getWorldMatrix() * m_localMatrix : m_localMatrix;
		m_worldMatrix.decompose(&m_worldRotation, &m_worldPosition, &m_worldScale);
		// 遍历子节点
	}
	return m_worldMatrix;
}

void Transform::setValue(const String& name, const AnimValue& values, float weight /* = 1.0f */)
{
	// 修正数据
}

CU_NS_END
