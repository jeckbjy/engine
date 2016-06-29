#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class CU_D3D11_API D3D11_InputLayout : public InputLayout
{
public:
	D3D11_InputLayout(uint32_t id, const InputElement* elements, size_t count);
	~D3D11_InputLayout();

	ID3D11InputLayout* createLayout(ID3D11DeviceN* device, ID3DBlob* code);

	uint32_t getID() const { return m_id; }

private:
	D3D11_INPUT_ELEMENT_DESC* m_desc;
	UINT		m_count;
	uint32_t	m_id;
};

CU_NS_END