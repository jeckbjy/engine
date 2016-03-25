#pragma once
#include "D3D12_API.h"

CU_NS_BEGIN

class CU_D3D12_API D3D12InputLayout : public InputLayout
{
public:
	D3D12InputLayout(const InputElement* elements, size_t count);
	~D3D12InputLayout();

	void fillLayout(D3D12_INPUT_LAYOUT_DESC& desc);

private:
	D3D12_INPUT_ELEMENT_DESC* m_desc;
};

CU_NS_END