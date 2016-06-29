#include "D3D11_InputLayout.h"
#include "D3D11_Mapping.h"
#include "D3D11_Device.h"
#include "D3D11_Program.h"

CU_NS_BEGIN

D3D11_InputLayout::D3D11_InputLayout(uint32_t id, const InputElement* elements, size_t count)
	: InputLayout(elements, count)
	, m_count(count)
	, m_id(id)
{
	m_desc = new D3D11_INPUT_ELEMENT_DESC[count];
	for (size_t i = 0; i < count; ++i)
	{
		D3D11_INPUT_ELEMENT_DESC& desc = m_desc[i];
		const InputElement& elem = m_elements[i];
		D3D11_Mapping::fillSemantic(elem.semantic, desc.SemanticName, desc.SemanticIndex);
		desc.Format = D3D11_Mapping::getFormat(elem.format);
		desc.InputSlot = elem.slot;
		desc.AlignedByteOffset = elem.offset;
		desc.InputSlotClass = (D3D11_INPUT_CLASSIFICATION)elem.rate;
		desc.InstanceDataStepRate = 0;	// 目前没用
	}
}

D3D11_InputLayout::~D3D11_InputLayout()
{
	delete[] m_desc;
}

ID3D11InputLayout* D3D11_InputLayout::createLayout(ID3D11DeviceN* device, ID3DBlob* code)
{
	if (code == NULL)
		return NULL;

	ID3D11InputLayout* layout = NULL;
	HRESULT hr = device->CreateInputLayout(m_desc, m_count, code->GetBufferPointer(), code->GetBufferSize(), &layout);
	if (FAILED(hr))
		return NULL;

	return layout;
}

CU_NS_END