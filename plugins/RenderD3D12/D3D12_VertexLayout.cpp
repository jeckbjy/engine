#include "D3D12_VertexLayout.h"
#include "D3D12_Mapping.h"

CUTE_NS_BEGIN

D3D12_VertexLayout::D3D12_VertexLayout(uint32_t id, const VertexElement* elements, size_t count)
	: VertexLayout(id, elements, count)
	, m_id(0)
{
	m_desc = new D3D12_INPUT_ELEMENT_DESC[count];
	for (size_t i = 0; i < count; ++i)
	{
		D3D12_INPUT_ELEMENT_DESC& desc = m_desc[i];
		const VertexElement& elem = elements[i];
		// convert
		D3D12_Mapping::fillSemantic(elem.semantic, desc.SemanticName, desc.SemanticIndex);
		desc.Format = D3D12_Mapping::getFormat(elem.format);
		desc.AlignedByteOffset = elem.offset;
		desc.InputSlot = elem.slot;
		desc.InputSlotClass = (D3D12_INPUT_CLASSIFICATION)elem.rate;
		desc.InstanceDataStepRate = 0;
	}
}

D3D12_VertexLayout::~D3D12_VertexLayout()
{
	delete m_desc;
	m_desc = NULL;
}

void D3D12_VertexLayout::fillLayout(D3D12_INPUT_LAYOUT_DESC& desc)
{
	desc.NumElements = m_elements.size();
	desc.pInputElementDescs = m_desc;
}

CUTE_NS_END
