#include "D3D11_VertexLayout.h"
#include "D3D11_Mapping.h"
#include "Singleton.h"

CU_NS_BEGIN

class LayoutMgr : public Singleton<LayoutMgr>
{
public:
	typedef std::multimap<uint32_t, D3D11VertexDeclaration*> DeclarationMap;

	D3D11VertexDeclaration* find(const VertexDeclaration& decl)
	{
		uint32_t code = decl.hash_code();
		DeclarationMap::iterator itor = _declarations.find(code);
		for (; itor != _declarations.end(); ++itor)
		{
			if (itor->first != code)
				break;
			if (itor->second->getDecl() == decl)
			{
				itor->second->retain();
				return itor->second;
			}
		}
		// 没有找到，创建一个
		D3D11VertexDeclaration* new_decl = new D3D11VertexDeclaration(decl);
		new_decl->retain();
		_declarations.insert(std::make_pair(code, new_decl));
		// 返回
		new_decl->retain();
		return new_decl;
	}
	void free(D3D11VertexDeclaration* decl)
	{
		Ref::ref_t refs = decl->unlink();
		if (refs > 1)
			return;
		uint32_t code = decl->getDecl().hash_code();
		// 释放
		DeclarationMap::iterator itor = _declarations.find(code);
		for (; itor != _declarations.end(); ++itor)
		{
			if (itor->first != code)
				break;
			if (itor->second == decl)
			{
				_declarations.erase(itor);
				break;
			}
		}
		delete decl;
	}
private:
	DeclarationMap _declarations;
};

D3D11VertexDeclaration::D3D11VertexDeclaration(const VertexDeclaration& decl)
	:_decl(decl)
{
	_desc.resize(decl.size());
	for (size_t i = 0; i < decl.size(); ++i)
	{
		const VertexElement& el_decl = _decl[i];
		D3D11_INPUT_ELEMENT_DESC& el_desc = _desc[i];
		el_desc.SemanticName = D3D11Mapping::getSemantic(el_decl.usage, el_desc.SemanticIndex);
	}
}

D3D11VertexLayout::D3D11VertexLayout(VertexDeclaration& desc)
	:VertexLayout(desc)
{
	_decl = LayoutMgr::Instance().find(desc);
}

D3D11VertexLayout::~D3D11VertexLayout()
{

}

void D3D11VertexLayout::bind(ID3D11DeviceContextN* context, Program* prog)
{
	ID3D11InputLayout* layout = NULL;
	// 查找
	int32_t prog_id = 0;
	LayoutMap::iterator itor = _layouts.find(prog_id);
	if (itor == _layouts.end())
	{// 创建？？
		//gD3D11Device->CreateInputLayout(&_desc[0], _desc.size(), prog);
	}
	else
	{
		layout = itor->second;
	}
	//if (_dirty)
	//{
	//	_buffers.clear();
	//	// 重新构建
	//}

	//context->IASetVertexBuffers(0, _buffers.size(), &_buffers[0], NULL, NULL);
	context->IASetInputLayout(layout);
}

CU_NS_END