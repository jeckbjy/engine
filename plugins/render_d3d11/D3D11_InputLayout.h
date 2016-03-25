#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

// gD3D11Device->CreateInputLayout(&_desc[0], _desc.size(), prog);
// todo:Graphics÷–¥¥Ω®
class CU_D3D11_API D3D11InputLayout : public InputLayout
{
public:
	D3D11InputLayout(const InputElement* elements, size_t count);
	~D3D11InputLayout();

private:
	D3D11_INPUT_ELEMENT_DESC* m_desc;
};

CU_NS_END