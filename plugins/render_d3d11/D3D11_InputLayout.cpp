#include "D3D11_InputLayout.h"
#include "D3D11_Mapping.h"

CU_NS_BEGIN

D3D11InputLayout::D3D11InputLayout(const InputElement* elements, size_t count)
	: InputLayout(elements, count)
{
	m_desc = new D3D11_INPUT_ELEMENT_DESC[count];
	for (size_t i = 0; i < count; ++i)
	{
		D3D11_INPUT_ELEMENT_DESC& desc = m_desc[i];
		const InputElement& elem = m_elements[i];
		D3D11Mapping::fillSemantic(elem.semantic, desc.SemanticName, desc.SemanticIndex);
		desc.Format = D3D11Mapping::getFormat(elem.format);
		desc.InputSlot = elem.slot;
		desc.AlignedByteOffset = elem.offset;
		desc.InputSlotClass = (D3D11_INPUT_CLASSIFICATION)elem.rate;
		desc.InstanceDataStepRate = 0;	// 目前没用
	}
}

D3D11InputLayout::~D3D11InputLayout()
{
	delete[] m_desc;
}

CU_NS_END