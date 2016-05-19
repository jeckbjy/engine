#include "D3D12_Program.h"

CU_NS_BEGIN

D3D12Program::D3D12Program()
	: m_blob(NULL)
{
}

D3D12Program::~D3D12Program()
{
	D3D12_RELEASE(m_blob);
}

bool D3D12Program::compile(const ProgramDesc& desc)
{
	D3D12_RELEASE(m_blob);
	
	ID3D10Blob* error = NULL;

	std::vector<D3D_SHADER_MACRO> macros;
	LPCSTR file = desc.file.empty() ? NULL : desc.file.c_str();
	if (!desc.macros.empty())
	{// split macros
	}

	macros.push_back({ NULL, NULL });
	UINT flags = 0;
	// todo:process includes ID3DInclude
	// 编译
	//D3DCompile2()
	HRESULT hr = D3DCompile(
		desc.code.c_str(),
		desc.code.size(),
		file,
		macros.data(),
		NULL,
		desc.entry.c_str(),
		desc.profile.c_str(),
		flags,
		0,
		&m_blob,
		&error);
	// 是否需要解析？？
	return SUCCEEDED(hr);
}

CU_NS_END