#include "Transform.h"
#include "Entity.h"
#include "Matrix3.h"

CU_NS_BEGIN

enum
{
	DIRTY_LOCAL		= 0x01,
	DIRTY_WORLD		= 0x02,
};

Transform::Transform()
{

}

Transform::Transform(const Quaternion& rotation, const Vector3& position, const Vector3& scale, TransformSpace space /* = TS_LOCAL */)
{

}

Transform::~Transform()
{

}

Transform* Transform::getChild(const String& name, bool recursive /* = false */) const
{
	return NULL;
}

void Transform::translate(const Vector3& vec, TransformSpace space /* = TS_LOCAL */)
{
	if (space == TS_LOCAL)
	{
		setPosition(m_position + vec);
	}
	else
	{// 相对全局坐标系
		setWorldPosition(getWorldPosition() + vec);
	}
}

void Transform::rotate(const Quaternion& q)
{
	// Note the order of the mult, i.e. q comes after

	// Normalize the quat to avoid cumulative problems with precision
	Quaternion qnorm = q;
	qnorm.normalize();
	setRotation(qnorm * m_rotation);
}

void Transform::rotate(const Vector3& axis, float angle)
{
	Quaternion q;
	q.fromAxisAngle(axis, angle);
	rotate(q);
}

//void Transform::scale(const Vector3& s, TransformSpace space /* = TS_LOCAL */)
//{
//
//}

void Transform::lookAt(const Vector3& location, const Vector3& up /* = Vector3::UNIT_Y */)
{
	Vector3 forward = location - getWorldPosition();

	Quaternion rotation = getWorldRotation();
	rotation.lookRotation(forward, up);
	setWorldRotation(rotation);
}

void Transform::roll(float angle)
{
	// Rotate around local Z axis
	Vector3 zAxis = m_rotation.rotate(Vector3::UNIT_Z);
	rotate(zAxis, angle);
}

void Transform::yaw(float angle)
{
	Vector3 yAxis = m_rotation.rotate(Vector3::UNIT_Y);
	rotate(yAxis, angle);
}

void Transform::pitch(float angle)
{
	// Rotate around local X axis
	Vector3 xAxis = m_rotation.rotate(Vector3::UNIT_X);
	rotate(xAxis, angle);
}

void Transform::setRotation(const Quaternion& r)
{
	m_rotation = r;
	m_flags[DIRTY_LOCAL] = true;
	markWorldDirty();
}

void Transform::setPosition(const Vector3& t)
{
	m_position = t;
	m_flags[DIRTY_LOCAL] = true;
	markWorldDirty();
}

void Transform::setScale(const Vector3& s)
{
	m_scale = s;
	m_flags[DIRTY_LOCAL] = true;
	markWorldDirty();
}

void Transform::setWorldPosition(const Vector3& pos)
{
	if (m_parent != NULL)
	{
		Vector3 invScale = 1/ m_parent->getWorldScale();
		Quaternion invRotation = m_parent->getWorldRotation().inverse();
		m_position = invRotation.rotate(pos - m_parent->getWorldPosition()) * invScale;
	}
	else
	{
		m_position = pos;
	}
	m_flags[DIRTY_LOCAL] = true;
	markWorldDirty();
}

void Transform::setWorldRotation(const Quaternion& rotation)
{
	if (m_parent)
	{
		Quaternion invRotation = m_parent->getWorldRotation().inverse();
		m_rotation = invRotation * rotation;
	}
	else
	{
		m_rotation = rotation;
	}
	m_flags[DIRTY_LOCAL] = true;
	markWorldDirty();
}

void Transform::setWorldScale(const Vector3& scale)
{
	if (m_parent != nullptr)
	{
		Matrix3 rotScale;
		m_parent->getWorldMatrix().toMatrix3(rotScale);
		rotScale.inverse();

		Matrix3 scaleMat = Matrix3(Quaternion::IDENTITY, scale);
		scaleMat = rotScale * scaleMat;

		Quaternion rotation;
		Vector3 localScale;
		scaleMat.decomposition(rotation, localScale);

		m_scale = localScale;
	}
	else
	{
		m_scale = scale;
	}
	m_flags[DIRTY_LOCAL] = true;
	markWorldDirty();
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

const Matrix4& Transform::getLocalMatrix() const
{
	// 处理
	if (m_flags[DIRTY_LOCAL])
	{
		m_flags.reset(DIRTY_LOCAL);
		m_localMatrix.setTransform(m_position, m_scale, m_rotation);
	}

	return m_localMatrix;
}

const Matrix4& Transform::getWorldMatrix() const
{
	updateWorldTransform();
	return m_worldMatrix;
}

void Transform::updateWorldTransform() const
{
	if (!m_flags[DIRTY_WORLD])
		return;
	m_flags.reset(DIRTY_WORLD);

	if (m_parent)
	{
		// update orientation
		const Quaternion& parentRotation = m_parent->getWorldRotation();
		m_worldRotation = parentRotation * m_rotation;

		// Scale own position by parent scale, just combine as equivalent axes, no shearing
		const Vector3 parentScale = m_parent->getWorldScale();
		m_worldScale = parentScale * m_scale;

		// Change position vector based on parent's orientation & scale
		m_worldPosition = parentRotation.rotate(parentScale * m_position);

		// Add altered position vector to parents
		m_worldPosition += m_parent->getWorldPosition();

		m_worldMatrix.setTransform(m_worldPosition, m_worldScale, m_worldRotation);
	}
	else
	{
		m_worldRotation = m_rotation;
		m_worldPosition = m_position;
		m_worldScale = m_scale;
		m_worldMatrix = getLocalMatrix();
	}
}

void Transform::markWorldDirty()
{
	m_flags[DIRTY_WORLD] = true;
	// todo:递归设置children
}

void Transform::setValue(const String& name, const AnimValue& values, float weight /* = 1.0f */)
{
	// 修正数据
}

CU_NS_END
