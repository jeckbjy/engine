#pragma once
#include "D3D11_API.h"
#include <map>

CU_NS_BEGIN

class CU_D3D11_API D3D11VertexDeclaration : public Ref
{
public:
	typedef std::vector<D3D11_INPUT_ELEMENT_DESC> InputElementList;

	D3D11VertexDeclaration(const VertexDeclaration& decl);

	const VertexDeclaration& getDecl() const { return _decl;  }
	const InputElementList& getDesc() const { return _desc; }
private:
	VertexDeclaration	_decl;
	InputElementList	_desc;
};

class CU_D3D11_API D3D11VertexLayout : public VertexLayout
{
public:
	D3D11VertexLayout(VertexDeclaration& desc);
	~D3D11VertexLayout();

	void bind(ID3D11DeviceContextN* context, Program* prog);

private:
	typedef std::map<int32_t, ID3D11InputLayout*> LayoutMap;
	typedef std::vector<ID3D11Buffer*> BufferList;
	D3D11VertexDeclaration* _decl;
	LayoutMap	_layouts;
	BufferList	_buffers;
};

CU_NS_END