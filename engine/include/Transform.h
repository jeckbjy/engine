#pragma once
#include "API.h"
#include "List.h"
#include "Bits.h"
#include "Matrix4.h"
#include "Component.h"

CU_NS_BEGIN

enum TransformSpace
{
	TS_LOCAL,	// 局部坐标系
	TS_WORLD,	// 世界坐标系
};

class CU_API Transform : public Component, public ListNode<Transform, 2>
{
	DECLARE_RTTI(Transform, Component, "TRAN")
public:
	Transform();
	Transform(const Quaternion& rotation, const Vector3& position, const Vector3& scalar, TransformSpace space = TS_LOCAL);
	virtual ~Transform();

	Transform* getChild(const String& name, bool recursive = false) const;

	void translate(const Vector3& vec, TransformSpace space = TS_LOCAL);
	void rotate(const Quaternion& q);
	void rotate(const Vector3& axis, float angle);
	//void scale(const Vector3& s, TransformSpace space = TS_LOCAL);

	void lookAt(const Vector3& location, const Vector3& up = Vector3::UNIT_Y);
	void roll(float angle);
	void yaw(float angle);
	void pitch(float angle);

	// 默认局部坐标
	void setRotation(const Quaternion& r);
	void setPosition(const Vector3& t);
	void setScale(const Vector3& s);

	void setWorldPosition(const Vector3& position);
	void setWorldRotation(const Quaternion& rotation);
	void setWorldScale(const Vector3& scale);

	const Quaternion& getRotation() const { return m_rotation; }
	const Vector3&	  getPosition() const { return m_position; }
	const Vector3&	  getScale() const { return m_scale; }

	const Quaternion& getWorldRotation() const;
	const Vector3&    getWorldPosition() const;
	const Vector3&    getWorldScale() const;

	const Matrix4&	  getLocalMatrix() const;
	const Matrix4&    getWorldMatrix() const;

	Vector3 getForward() const { return getWorldRotation().rotate(-Vector3::UNIT_Z); }
	Vector3 getUp() const { return getWorldRotation().rotate(Vector3::UNIT_Y); }
	Vector3 getRight() const { return getWorldRotation().rotate(Vector3::UNIT_X); }

	virtual void setValue(const String& name, const AnimValue& values, float weight = 1.0f);

protected:
	void markWorldDirty();
	void updateWorldTransform() const;

protected:
	typedef List<Transform, 2> ChildList;
	// Transform
	Quaternion			m_rotation;
	Vector3				m_position;
	Vector3				m_scale;
	mutable Quaternion  m_worldRotation;
	mutable Vector3     m_worldPosition;
	mutable Vector3     m_worldScale;
	mutable Matrix4		m_localMatrix;
	mutable Matrix4		m_worldMatrix;
	mutable Mask8		m_flags;
	mutable uint32		m_hash;

	// Hierarchy
	Transform*			m_parent;
	ChildList			m_children;
};

CU_NS_END