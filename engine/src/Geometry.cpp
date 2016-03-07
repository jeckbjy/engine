#include "Geometry.h"

CU_NS_BEGIN

Geometry::Geometry()
: m_vertex(NULL), m_index(NULL)
, m_distance(0.0f)
{
}

Geometry::~Geometry()
{

}

void Geometry::draw(CommandBuffer* cmd)
{
	if (!m_vertex)
		return;

	cmd->setVertexLayout(m_vertex);
	cmd->draw(m_params);
}

void Geometry::setLodDistance(float distance)
{
	m_distance = distance < 0.0f ? 0.0f : distance;
}

void Geometry::setVertexBuffers(VertexLayout* buffers)
{
	m_vertex = buffers;
}

void Geometry::setIndexBuffer(IndexBuffer* buffer)
{
	m_index = buffer;
}

void Geometry::setDrawParam(Topology type, size_t indexStart, size_t indexCount)
{
	m_params.type = type;
	m_params.indexStart = indexStart;
	m_params.indexCount = indexCount;

	if (indexCount > 0)
	{
		m_params.vertexCount = m_vertex ? m_vertex->getVertexCount() : 0;
	}
	else
	{
		m_params.vertexCount = 0;
	}
}

void Geometry::setDrawParam(Topology type, size_t indexStart, size_t indexCount, size_t vertexStart, size_t vertexCount, size_t instCount)
{
	m_params.type = type;
	m_params.indexStart = indexStart;
	m_params.indexCount = indexCount;
	m_params.vertexStart = vertexStart;
	m_params.vertexCount = vertexCount;
	m_params.instanceCount = instCount;
}

CU_NS_END
