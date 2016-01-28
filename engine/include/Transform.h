#pragma once
#include "API.h"
#include "Tree.h"
#include "Bits.h"
#include "Matrix4.h"
#include "Component.h"

CU_NS_BEGIN

enum TransformSpace
{
	TS_LOCAL,	// 局部坐标系
	TS_WORLD,	// 世界坐标系
};

class Entity;
class CU_API Transform : public Component, public TreeNode<Transform>
{
	DECLARE_RTTI(Transform, Component, "TRAN")
public:
	Transform();
	Transform(const Quaternion& rotation, const Vector3& position, const Vector3& scalar, TransformSpace space = TS_LOCAL);
	virtual ~Transform();

	Entity* findEntity(const String& name) const;
	Entity* findEntityByPath(const String& path) const;

	Transform* getChild(const String& name, bool recursive = false) const;

	void translate(const Vector3& position, TransformSpace space = TS_LOCAL);
	void rotate(const Quaternion& rotation, TransformSpace space = TS_LOCAL);
	void scale(const Vector3& s, TransformSpace space = TS_LOCAL);

	// 默认局部坐标
	void setRotation(const Quaternion& r);
	void setPosition(const Vector3& t);
	void setScale(const Vector3& s);

	void setLocalRotation(const Quaternion& r);
	void setLocalPosition(const Vector3& t);
	void setLocalScale(const Vector3& s);

	void setWorldRotation(const Quaternion& r);
	void setWorldPosition(const Vector3& t);
	void setWorldScale(const Vector3& s);

	const Quaternion& getLocalRotation() const;
	const Vector3&	  getLocalPosition() const;
	const Vector3&	  getLocalScale() const;
	const Matrix4&	  getLocalMatrix() const;

	const Quaternion& getWorldRotation() const;
	const Vector3&    getWorldPosition() const;
	const Vector3&    getWorldScale() const;
	const Matrix4&    getWorldMatrix() const;

	virtual void setValue(const String& name, const AnimValue& values, float weight = 1.0f);

protected:
	Quaternion m_localRotation;
	Vector3    m_localPosition;
	Vector3    m_localScale;
	mutable Quaternion  m_worldRotation;
	mutable Vector3     m_worldPosition;
	mutable Vector3     m_worldScale;
	mutable Matrix4		m_localMatrix;
	mutable Matrix4		m_worldMatrix;
	mutable Mask8		m_flags;
};

CU_NS_END