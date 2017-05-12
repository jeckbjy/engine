#include "D3D12_Shader.h"

CUTE_NS_BEGIN

//D3D12_Shader::D3D12_Shader(uint32_t id)
//	: ShaderStage(id)
//	, m_code(NULL)
//{
//}
//
//D3D12_Shader::~D3D12_Shader()
//{
//	D3D12_RELEASE(m_code);
//}
//
//bool D3D12_Shader::compile(const ShaderDesc& desc)
//{
//	D3D12_RELEASE(m_code);
//	
//	ID3D10Blob* error = NULL;
//
//	std::vector<D3D_SHADER_MACRO> macros;
//	LPCSTR file = desc.file.empty() ? NULL : desc.file.c_str();
//	if (!desc.macros.empty())
//	{// split macros
//	}
//
//	macros.push_back({ NULL, NULL });
//	UINT flags = 0;
//	// todo:process includes ID3DInclude
//	// 编译
//	//D3DCompile2()
//	HRESULT hr = D3DCompile(
//		desc.code.c_str(),
//		desc.code.size(),
//		file,
//		macros.data(),
//		NULL,
//		desc.entry.c_str(),
//		desc.profile.c_str(),
//		flags,
//		0,
//		&m_code,
//		&error);
//	// 是否需要解析？？
//	return SUCCEEDED(hr);
//}
//
//void D3D12_Shader::parse()
//{
//	HRESULT hr;
//	ID3D12ShaderReflection* reflection;
//	D3D12_SHADER_DESC shader_desc;
//	hr = D3DReflect(m_code->GetBufferPointer(), m_code->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflection);
//	if (FAILED(hr))
//		return;
//
//	hr = reflection->GetDesc(&shader_desc);
//	if (FAILED(hr))
//		return;
//
//	for (UINT i = 0; i < shader_desc.BoundResources; ++i)
//	{
//		D3D12_SHADER_INPUT_BIND_DESC bind_desc;
//		hr = reflection->GetResourceBindingDesc(i, &bind_desc);
//		D3D12_CHECK(hr, "GetResourceBindingDesc fail!");
//		for (UINT j = 0; j < bind_desc.BindCount; ++j)
//		{
//
//		}
//	}
//
//	ID3D12ShaderReflectionConstantBuffer* constant;
//	ID3D12ShaderReflectionVariable*	variable;
//	D3D12_SHADER_BUFFER_DESC buf_desc;
//	for (UINT i = 0; i < shader_desc.ConstantBuffers; ++i)
//	{
//		constant = reflection->GetConstantBufferByIndex(i);
//		hr = constant->GetDesc(&buf_desc);
//		if (FAILED(hr))
//			continue;
//		for (UINT j = 0; j < buf_desc.Variables; ++j)
//		{
//			variable = constant->GetVariableByIndex(j);
//			// 解析
//		}
//	}
//
//	D3D12_RELEASE(reflection);
//}

CUTE_NS_END
