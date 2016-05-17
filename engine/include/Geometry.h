#pragma once
#include "Object.h"
#include "Graphics.h"

CU_NS_BEGIN

// »æÖÆ²ÎÊý
struct CU_API DrawParam
{
	Topology	type;
	size_t		vertexStart;
	size_t		vertexCount;
	size_t		indexStart;
	size_t		indexCount;
	size_t		instanceCount;
	DrawParam()
		: type(PT_TRIANGLE_LIST)
		, vertexStart(0)
		, vertexCount(0)
		, indexStart(0)
		, indexCount(0)
		, instanceCount(0)
	{}

	bool isInstanced() const { return instanceCount > 0; }
	bool isIndexed() const { return indexCount > 0; }
};

class VertexLayout;
class CommandBuffer;
/// Defines one or more vertex buffers, an index buffer and a draw range.
class CU_API Geometry : public Object
{
public:
	Geometry();
	virtual ~Geometry();

	void draw(CommandBuffer* cmd);

	void setLodDistance(float distance);
	//void setVertexBuffers(VertexLayout* buffers);
	void setIndexBuffer(IndexBuffer* buffer);
	void setDrawParam(Topology type, size_t indexStart, size_t indexCount);
	void setDrawParam(Topology type, size_t indexStart, size_t indexCount, size_t vertexStart, size_t vertexCount, size_t instCount = 0);

	//VertexLayout*	getVertexBuffers() { return m_vertex; }
	IndexBuffer*	getIndexBuffer() { return m_index; }
	const DrawParam& getDrawParam() const { return m_params; }
	float			getLodDistance() const { return m_distance; }

protected:
	InputLayout*	m_vertex;
	IndexBuffer*	m_index;
	DrawParam		m_params;
	float			m_distance;		// lod
};

CU_NS_END
