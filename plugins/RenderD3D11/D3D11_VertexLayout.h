#pragma once
#include "D3D11_API.h"

CUTE_NS_BEGIN

class CUTE_D3D11_API D3D11_VertexLayout : public VertexLayout
{
public:
	D3D11_VertexLayout(uint32_t id, const VertexElement* elements, size_t count);
	~D3D11_VertexLayout();

	ID3D11InputLayout* createLayout(ID3D11DeviceN* device, ID3DBlob* code);

	uint32_t getID() const { return m_id; }

private:
	D3D11_INPUT_ELEMENT_DESC* m_desc;
	UINT		m_count;
};

CUTE_NS_END
