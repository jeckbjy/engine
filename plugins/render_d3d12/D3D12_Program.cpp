#include "D3D12_Program.h"

CU_NS_BEGIN

D3D12Program::D3D12Program()
	: m_blob(NULL)
	, m_error(NULL)
{
}

D3D12Program::~D3D12Program()
{
	release();
}

void D3D12Program::release()
{
	D3D12_RELEASE(m_blob);
	D3D12_RELEASE(m_error);
}

bool D3D12Program::compile(const ProgramDesc& desc)
{
	std::vector<D3D_SHADER_MACRO> macros;
	release();
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
		&m_error);
	// 是否需要解析？？
	return SUCCEEDED(hr);
}

CU_NS_END