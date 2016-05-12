#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class CU_D3D11_API D3D11InputLayout : public InputLayout
{
public:
	D3D11InputLayout(const InputElement* elements, size_t count);
	~D3D11InputLayout();

	ID3D11InputLayout* getLayout(D3D11Program* prog);

private:
	// program不能释放再创建，否则会导致Layout无限增加
	// 需要不需要LayoutManager中同一管理呢？？
	typedef std::map<uint, ID3D11InputLayout*>	LayoutMap;
	D3D11_INPUT_ELEMENT_DESC* m_desc;
	UINT		m_count;
	LayoutMap	m_layouts;
};

CU_NS_END