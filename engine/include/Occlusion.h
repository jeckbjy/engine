#pragma once
#include "API.h"
#include "Object.h"
#include "Matrix4.h"
#include "GraphicsDefs.h"

CU_NS_BEGIN

// ÕÚµ²ÌÞ³ý
class CU_API Occlusion : public Object
{
public:
	Occlusion();
	~Occlusion();

protected:
	int*	m_buffer;
	int		m_width;
	int		m_height;
	size_t	m_numTriangles;
	size_t	m_maxTriangles;
	CullMode m_cull;
	bool	m_depthHierarchyDirty;
	bool	m_reverseCulling;
	Matrix4	m_projection;
};

CU_NS_END
