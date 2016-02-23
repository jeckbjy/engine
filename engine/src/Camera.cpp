#include "Camera.h"

CU_NS_BEGIN

enum
{
	DIRTY_VIEW		= 0x04,
	DIRTY_PROJ		= 0x08,
	DIRTY_FRUSTUM	= 0x10,
	DIRTY_ALL		= 0xFE,
	DIRTY_PROJ_FRUS = DIRTY_PROJ | DIRTY_FRUSTUM,
};

Camera::Camera()
{

}

Camera::~Camera()
{

}

void Camera::setNearClip(float nearClip)
{
	m_near = nearClip;
	m_flag.set(DIRTY_PROJ_FRUS);
}

void Camera::setFarClip(float farClip)
{
	m_far = farClip;
	m_flag.set(DIRTY_PROJ_FRUS);
}

void Camera::setFov(float fov)
{
	m_fov = fov;
	m_flag.set(DIRTY_PROJ_FRUS);
}

void Camera::setOrthoSize(float size, float aspect)
{
	m_size = size;
	m_aspect = aspect;
	m_flag.set(DIRTY_PROJ_FRUS);
}

void Camera::setOrthoSize(const Vector2& size)
{
	m_size = size.y;
	m_aspect = size.x / size.y;
	m_flag.set(DIRTY_PROJ_FRUS);
}

void Camera::setAspect(float aspect)
{
	m_aspect = aspect;
	m_flag.set(DIRTY_PROJ_FRUS);
}

void Camera::setZoom(float zoom)
{
	m_zoom = zoom;
	m_flag.set(DIRTY_PROJ_FRUS);
}

void Camera::setLodBias(float bias)
{
	m_lodBias = bias;
}

void Camera::setFlipVertical(bool enable)
{
	m_flag.set(FLAG_FLIP | DIRTY_PROJ);
}

const Matrix4& Camera::getView() const
{
	if (m_flag[DIRTY_VIEW])
	{
		m_flag.reset(DIRTY_VIEW);
	}
	return m_view;
}

const Matrix4& Camera::getProjection() const
{
	if (m_flag[DIRTY_PROJ])
	{
		getProjection(m_projection, false);
		m_flag.reset(DIRTY_PROJ);
	}
	return m_projection;
}

const Frustum& Camera::getFrustum() const
{
	if (m_flag[DIRTY_FRUSTUM])
	{
		m_flag.reset(DIRTY_FRUSTUM);
	}
	return m_frustum;
}

void Camera::getProjection(Matrix4& mat, bool ogl_format) const
{
	if (!isOrthographic())
	{
		float h = 1.0f / tanf(MATH_DEG_TO_RAD(m_fov * 0.5f)) * m_zoom;
		float w = h / m_aspect;
		float q, r;
		if (ogl_format)
		{
			q = (m_far + m_near) / (m_far - m_near);
			r = -2.0f * m_far * m_near / (m_far - m_near);
		}
		else
		{
			q = m_far / (m_far - m_near);
			r = -q * m_near;
		}
		mat.set(
			w, 0, m_offset.x * 2.0f, 0,
			0, h, 0, 0,
			0, 0, q, r,
			0, 0, 1.0f, 0
			);
	}
	else
	{
		// Disregard near clip, because it does not affect depth precision as with perspective projection
		float h = (1.0f / (m_size * 0.5f)) * m_zoom;
		float w = h / m_aspect;
		float q, r;
		if (ogl_format)
		{
			q = 2.0f / m_far;
			r = -1.0f;
		}
		else
		{
			q = 1.0f / m_far;
			r = 0.0f;
		}
		mat.set(
			0, 0, 0, m_offset.x * 2.0f,
			0, h, 0, m_offset.y * 2.0f,
			0, 0, q, r,
			0, 0, 0, 1.0f
			);
	}
	static const Matrix4 flipMatrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
	if (isFlipVertical())
		mat = flipMatrix * mat;
}

float Camera::getDistance(const Vector3& pos) const
{
	if (isOrthographic())
	{
		return 0;
	}
	else
	{
		return 0;
	}
}

float Camera::getLodDistance(float distance, float scale, float bias) const
{
	return 0;
}

RenderPath* Camera::getRenderPath()
{
	return m_render;
}

CU_NS_END