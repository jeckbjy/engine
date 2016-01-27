#pragma once
#include "API.h"
#include "GraphicsDefs.h"
#include "PixelFormat.h"

CU_NS_BEGIN

// 顶点信息
struct CU_API VertexElement
{
	VertexUsage	usage;	// Semantic
	PixelFormat type;	// 类型
	uint8_t		slot;	// 槽位
	uint8_t		offset;	// 偏移
	uint8_t		stride;	// 跨度,相同slot应该有相同跨度

	bool operator ==(const VertexElement& other) const
	{
		return this->usage == other.usage && this->type == other.type && this->slot == other.slot;
	}
	bool operator !=(const VertexElement& other) const
	{
		return	this->usage != other.usage || this->type != other.type || this->slot != other.slot;
	}
	VertexElement() :usage(VU_POSITION), type(PF_FLOAT3), slot(0){}
	VertexElement(VertexUsage usage) :usage(usage), type(PF_UNKNOWN), slot(0){}
	VertexElement(VertexUsage usage, PixelFormat type, uint8_t slot = 0) :usage(usage), type(type), slot(slot){}
};

class CU_API VertexDeclaration
{
public:
	typedef std::vector<VertexElement> ElementVec;
	typedef ElementVec::const_iterator const_iterator;

	VertexDeclaration();
	VertexDeclaration(const VertexElement* elements, size_t num);
	void build();
	bool equal(const VertexDeclaration& other) const;
	void push_back(const VertexElement& ve);
	void push_back(VertexUsage usage, PixelFormat format = PF_UNKNOWN, uint8_t slot = 0);

	uint32_t hash_code() const { return m_code; }
	size_t size() const { return m_elements.size(); }

	const VertexElement& at(size_t i) const { return m_elements.at(i); }
	const VertexElement& operator[](size_t index) const { return m_elements[index]; }

	const_iterator begin() const { return m_elements.begin(); }
	const_iterator end() const { return m_elements.end(); }
	bool hasUsage(VertexUsage usage) const { return (m_flag & (1 << usage)) != 0; }
	int  getOffset(VertexUsage usage) const;
	uint32_t stride() const { return m_stride; }

	bool operator==(const VertexDeclaration& rhs) const;

private:
	ElementVec	m_elements;
	uint8_t		m_builded;
	uint32_t	m_flag;			// 标识所含有的usage
	uint32_t	m_code;
	uint32_t	m_stride;		// 跨度
};

class GpuBuffer;
typedef GpuBuffer VertexBuffer;
// 顶点结构
class CU_API VertexLayout : public Object
{
	DECLARE_RTTI(VertexLayout, Object, "VLAY");
public:
	VertexLayout(VertexDeclaration& desc);
	virtual ~VertexLayout(){}

	void setBuffer(VertexBuffer* vb, int slot = 0);
	void setStart(size_t off) { m_start = off; }

protected:
	typedef std::map<int, VertexBuffer*> BufferList;
	bool		m_dirty;
	size_t		m_start;
	BufferList	m_buffers;
};

CU_NS_END
