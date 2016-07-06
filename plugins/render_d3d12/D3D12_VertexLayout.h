#pragma once
#include "D3D12_API.h"

CU_NS_BEGIN

class CU_D3D12_API D3D12_VertexLayout : public VertexLayout
{
public:
	D3D12_VertexLayout(uint32_t id, const VertexElement* elements, size_t count);
	~D3D12_VertexLayout();

	void fillLayout(D3D12_INPUT_LAYOUT_DESC& desc);

	uint32_t getID() const { return m_id; }

private:
	D3D12_INPUT_ELEMENT_DESC* m_desc;
	uint32_t m_id;
};

CU_NS_END