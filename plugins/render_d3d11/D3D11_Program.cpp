#include "D3D11_Program.h"

CU_NS_BEGIN

D3D11Program::D3D11Program()
	: m_shader(NULL)
	, m_code(NULL)
{
}

D3D11Program::~D3D11Program()
{
	D3D11_RELEASE(m_code);
	D3D11_RELEASE(m_shader);
}

bool D3D11Program::compile(const ProgramDesc& desc)
{
	ID3DBlob* errors = NULL;
	UINT flags = 0;
	flags |= desc.rowMajor ? D3DCOMPILE_PACK_MATRIX_ROW_MAJOR : D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;
	flags |= D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;

	if (desc.debug)
	{
		flags |= D3DCOMPILE_DEBUG;
		flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	}

	HRESULT hr = D3DCompile(
		desc.code.c_str(),
		desc.code.size(),
		NULL, NULL, NULL,
		desc.entry.c_str(),
		desc.profile.c_str(),
		flags,
		0,
		&m_code,
		&errors);

	if (SUCCEEDED(hr))
	{
		DWORD* code = (DWORD*)m_code->GetBufferPointer();
		SIZE_T size = m_code->GetBufferSize();
		create(desc.stage, code, size);
	}

	// ½âÎöreflect

	if (desc.stage != SHADER_STAGE_VERTEX)
	{
		D3D11_RELEASE(m_code);
	}

	D3D11_RELEASE(errors);

	return SUCCEEDED(hr);
}

void D3D11Program::create(ShaderStage stage, DWORD* code, SIZE_T size)
{
	ID3D11Device* device = gD3D11Device()->getDevice();
	ID3D11ClassLinkage* linkage = NULL;

	HRESULT hr;
	switch (stage)
	{
	case SHADER_STAGE_VERTEX:
		hr = device->CreateVertexShader(code, size, linkage, (ID3D11VertexShader**)&m_shader);
		break;
	case SHADER_STAGE_HULL:
		hr = device->CreateHullShader(code, size, linkage, (ID3D11HullShader**)&m_shader);
		break;
	case SHADER_STAGE_DOMAIN:
		hr = device->CreateDomainShader(code, size, linkage, (ID3D11DomainShader**)&m_shader);
		break;
	case SHADER_STAGE_GEOMETRY:
		hr = device->CreateGeometryShader(code, size, linkage, (ID3D11GeometryShader**)&m_shader);
		break;
	case SHADER_STAGE_PIXEL:
		hr = device->CreatePixelShader(code, size, linkage, (ID3D11PixelShader**)&m_shader);
		break;
	case SHADER_STAGE_COMPUTE:
		hr = device->CreateComputeShader(code, size, linkage, (ID3D11ComputeShader**)&m_shader);
		break;
	}
}

CU_NS_END