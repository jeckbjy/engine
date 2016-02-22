#pragma once
#include "Component.h"
#include "Matrix4.h"
#include "Frustum.h"
#include "Vector2.h"
#include "Rect.h"
#include "Bits.h"
#include "RenderPath.h"

CU_NS_BEGIN

// 相当于一个View,渲染到Texture
// 可用于实现分屏,多加一个Camera
class CU_API Camera : public Component, public ListNode<Camera, 2>
{
	DECLARE_RTTI(Camera, Component, TYPE_CAMERA);
	enum 
	{ 
		FLAG_ORTHO		= 0x01, 
		FLAG_FLIP		= 0x02,
		FLAG_OCCLUSION	= 0x04,
		FLAG_HDR		= 0x08,
	};
public:
	Camera();
	~Camera();

	bool isOrthographic() const{ return m_flag[FLAG_ORTHO]; }
	bool isFlipVertical() const { return m_flag[FLAG_FLIP]; }

	void setNearClip(float nearClip);
	void setFarClip(float farClip);
	void setFov(float fov);
	void setOrthoSize(float size, float aspect = 1.0f);
	void setOrthoSize(const Vector2& size);
	void setAspect(float aspect);
	void setZoom(float zoom);
	void setLodBias(float bias);
	void setFlipVertical(bool enable);

	float getNearClip() const { return m_near; }
	float getFarClip() const { return m_far; }
	float getFov() const { return m_fov; }
	float getOrthoSize() const { return m_size; }
	float getAspectRatio() const { return m_aspect; }
	float getZoom() const { return m_zoom; }
	float getLodBias() const { return m_lodBias; }

	float getDistance(const Vector3& pos) const;
	float getLodDistance(float distance, float scale, float bias) const;

	//float getWidth() const { return 2 * m_ortho / m_aspect; }
	//float getHeight() const { return 2 * m_ortho; }

	const Matrix4& getView() const;
	const Matrix4& getProjection() const;
	const Frustum& getFrustum() const;

private:
	void getProjection(Matrix4& mat, bool ogl_format) const;

private:
	uint	m_cullMask;	// just or layer？？
	float	m_near;
	float	m_far;
	float	m_fov;		// field of view
	float	m_size;		// height/2
	float	m_aspect;	// x/y
	float	m_zoom;
	float	m_lodBias;	// 斜率
	Vector2 m_offset;
	RectI	m_viewport;	// 视窗,用于分屏,百分比
	RenderPath* m_render;	// 渲染通路？？
	// cached
	mutable Mask8	m_flag;
	mutable Matrix4 m_view;
	mutable Matrix4 m_projection;
	mutable Frustum m_frustum;
};

CU_NS_END