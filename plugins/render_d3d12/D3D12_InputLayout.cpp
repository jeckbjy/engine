#include "D3D12_InputLayout.h"
#include "D3D12_Mapping.h"

CU_NS_BEGIN

D3D12InputLayout::D3D12InputLayout(const InputElement* elements, size_t count)
	: InputLayout(elements, count)
{
	m_desc = new D3D12_INPUT_ELEMENT_DESC[count];
	for (size_t i = 0; i < count; ++i)
	{
		D3D12_INPUT_ELEMENT_DESC& desc = m_desc[i];
		const InputElement& elem = elements[i];
		// convert
		D3D12Mapping::fillSemantic(elem.semantic, desc.SemanticName, desc.SemanticIndex);
		desc.Format = D3D12Mapping::getFormat(elem.format);
		desc.AlignedByteOffset = elem.offset;
		desc.InputSlot = elem.slot;
		desc.InputSlotClass = (D3D12_INPUT_CLASSIFICATION)elem.rate;
		desc.InstanceDataStepRate = 0;
	}
}

D3D12InputLayout::~D3D12InputLayout()
{
	delete m_desc;
	m_desc = NULL;
}

void D3D12InputLayout::fillLayout(D3D12_INPUT_LAYOUT_DESC& desc)
{
	desc.NumElements = m_elements.size();
	desc.pInputElementDescs = m_desc;
}

CU_NS_END