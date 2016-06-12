#include "D3D11_Shader.h"
#include "D3D11_Device.h"

CU_NS_BEGIN

D3D11_Shader::D3D11_Shader(uint32_t id)
	: m_shader(NULL)
	, m_code(NULL)
{
}

D3D11_Shader::~D3D11_Shader()
{
	for (size_t i = 0; i < m_uniforms.size(); ++i)
	{
		delete m_uniforms[i];
	}
	m_uniforms.clear();

	D3D11_RELEASE(m_code);
	D3D11_RELEASE(m_shader);
}

bool D3D11_Shader::compile(const ShaderDesc& desc)
{
	ID3DBlob* errors = NULL;
	UINT flags = 0;
	bool rowMajor = true;
	bool debug = false;
	flags |= rowMajor ? D3DCOMPILE_PACK_MATRIX_ROW_MAJOR : D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;
	flags |= D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;

	if (debug)
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
		bool result = create(desc.stage, code, size);
		if (result)
		{
			parse();
		}
	}

	D3D11_RELEASE(errors);

	return SUCCEEDED(hr);
}

bool D3D11_Shader::create(ShaderType stage, DWORD* code, SIZE_T size)
{
	ID3D11DeviceN* device = gD3D11NativeDevice();
	ID3D11ClassLinkage* linkage = NULL;

	HRESULT hr;
	switch (stage)
	{
	case SHADER_VERTEX:
		hr = device->CreateVertexShader(code, size, linkage, (ID3D11VertexShader**)&m_shader);
		break;
	case SHADER_HULL:
		hr = device->CreateHullShader(code, size, linkage, (ID3D11HullShader**)&m_shader);
		break;
	case SHADER_DOMAIN:
		hr = device->CreateDomainShader(code, size, linkage, (ID3D11DomainShader**)&m_shader);
		break;
	case SHADER_GEOMETRY:
		hr = device->CreateGeometryShader(code, size, linkage, (ID3D11GeometryShader**)&m_shader);
		break;
	case SHADER_PIXEL:
		hr = device->CreatePixelShader(code, size, linkage, (ID3D11PixelShader**)&m_shader);
		break;
	case SHADER_COMPUTE:
		hr = device->CreateComputeShader(code, size, linkage, (ID3D11ComputeShader**)&m_shader);
		break;
	}

	return SUCCEEDED(hr);
}

void D3D11_Shader::parse()
{
	static const char* SHADER_NAME[SHADER_COUNT] = 
	{
		"VS", "HS", "DS", "GS", "PS", "CS"
	};

	HRESULT hr;
	ID3D11ShaderReflection* reflection;
	D3D11_SHADER_DESC shader_desc;
	hr = D3DReflect(m_code->GetBufferPointer(), m_code->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflection);
	if (FAILED(hr))
		return;
	hr = reflection->GetDesc(&shader_desc);
	if (FAILED(hr))
		return;

	typedef std::map<String, size_t> IndexMap;
	IndexMap indexMap;

	UniformType type;
	D3D11_SHADER_INPUT_BIND_DESC bindDesc;
	for (UINT i = 0; i < shader_desc.BoundResources; ++i)
	{
		hr = reflection->GetResourceBindingDesc(i, &bindDesc);
		D3D11_CHECK(hr, "GetResourceBindingDesc fail!");
		// 
		type = getInputType(bindDesc.Type, bindDesc.Dimension);
		if (type == UT_UNKNOWN)
			continue;

		UniformDesc* uniform = new UniformDesc();
		uniform->name = bindDesc.Name;
		uniform->slot = bindDesc.BindPoint;
		uniform->arrays = bindDesc.BindCount;	// like texture2d array
		uniform->type = type;

		// 每个CB都有独立的名字，不同的Stage不能重复
		if (uniform->type == UT_CBUFFER)
			uniform->name += SHADER_NAME[m_type];

		m_uniforms.push_back(uniform);
		indexMap[bindDesc.Name] = m_uniforms.size() - 1;
	}

	ID3D11ShaderReflectionConstantBuffer* constant;
	ID3D11ShaderReflectionVariable*	variable;

	D3D11_SHADER_BUFFER_DESC bufDesc;
	D3D11_SHADER_VARIABLE_DESC varDesc;
	D3D11_SHADER_TYPE_DESC typeDesc;
	size_t blockIndex;
	UniformDesc* block;
	for (UINT i = 0; i < shader_desc.ConstantBuffers; ++i)
	{
		constant = reflection->GetConstantBufferByIndex(i);
		hr = constant->GetDesc(&bufDesc);
		if (FAILED(hr))
			continue;
		IndexMap::const_iterator itor = indexMap.find(bufDesc.Name);
		if (itor == indexMap.end())
			continue;

		blockIndex = itor->second;
		block = m_uniforms[blockIndex];
		block->bytes = bufDesc.Size;

		for (UINT j = 0; j < bufDesc.Variables; ++j)
		{
			variable = constant->GetVariableByIndex(j);
			hr = variable->GetDesc(&varDesc);
			if (FAILED(hr))
				continue;
			hr = variable->GetType()->GetDesc(&typeDesc);
			if (FAILED(hr))
				continue;
			type = getVariableType(typeDesc);
			if (type == UT_UNKNOWN)
				continue;
			UniformDesc* uniform = new UniformDesc();
			uniform->name = varDesc.Name;
			uniform->index = blockIndex;
			uniform->arrays = typeDesc.Elements == 0 ? 1 : typeDesc.Elements;
			uniform->offset = varDesc.StartOffset;
			uniform->type = type;
			m_uniforms.push_back(uniform);
		}
	}

	D3D11_RELEASE(reflection);
}

UniformType D3D11_Shader::getInputType(D3D_SHADER_INPUT_TYPE type, D3D_SRV_DIMENSION dimension)
{
	switch (type)
	{
	case D3D_SIT_CBUFFER:
	case D3D_SIT_TBUFFER:
		return UT_CBUFFER;
	case D3D_SIT_SAMPLER:
		return UT_SAMPLER2D;
	case D3D_SIT_TEXTURE:
		switch (dimension)
		{
		case D3D_SRV_DIMENSION_TEXTURE1D:
			return UT_TEXTURE1D;
		case D3D_SRV_DIMENSION_TEXTURE2D:
			return UT_TEXTURE2D;
		case D3D_SRV_DIMENSION_TEXTURE3D:
			return UT_TEXTURE3D;
		case D3D_SRV_DIMENSION_TEXTURECUBE:
			return UT_TEXTURECUBE;
		case D3D_SRV_DIMENSION_TEXTURE2DMS:
			return UT_TEXTURECUBE;
		}
		break;
	case D3D_SIT_STRUCTURED:
		return UT_STRUCTURED_BUFFER;
	case D3D_SIT_BYTEADDRESS:
		return UT_BYTE_BUFFER;
	case D3D11_SIT_UAV_RWTYPED:
		switch (dimension)
		{
		case D3D_SRV_DIMENSION_TEXTURE1D:
			return UT_RWTEXTURE1D;
		case D3D_SRV_DIMENSION_TEXTURE2D:
			return UT_RWTEXTURE2D;
		case D3D_SRV_DIMENSION_TEXTURE3D:
			return UT_RWTEXTURE3D;
		case D3D_SRV_DIMENSION_TEXTURE2DMS:
			return UT_RWTEXTURE2DMS;
		case D3D_SRV_DIMENSION_BUFFER:
			return UT_RWTYPED_BUFFER;
		}
		break;
	case D3D11_SIT_UAV_RWSTRUCTURED:
		return UT_RWSTRUCTURED_BUFFER;
	case D3D11_SIT_UAV_RWBYTEADDRESS:
		return UT_RWBYTE_BUFFER;
	case D3D_SIT_UAV_APPEND_STRUCTURED:
		return UT_RWAPPEND_BUFFER;
	case D3D_SIT_UAV_CONSUME_STRUCTURED:
		return UT_RWCONSUME_BUFFER;
	case D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER:
		return UT_RWSTRUCTURED_BUFFER_WITH_COUNTER;
	default:
		return UT_UNKNOWN;
	}
	return UT_UNKNOWN;
}

UniformType D3D11_Shader::getVariableType(D3D11_SHADER_TYPE_DESC& desc)
{
	// 类型
	switch (desc.Class)
	{
	case D3D_SVC_SCALAR:
	{
		switch (desc.Type)
		{
		case D3D_SVT_BOOL:
			return UT_BOOL;
		case D3D_SVT_INT:
		case D3D_SVT_UINT:
			return UT_INT1;
		case D3D_SVT_FLOAT:
			return UT_FLOAT1;
		}
		break;
	}
	case D3D_SVC_VECTOR:
	{
		switch (desc.Type)
		{
		case D3D_SVT_INT:
			return UniformType(UT_INT1 + desc.Columns);
		case D3D_SVT_FLOAT:
			return UniformType(UT_FLOAT1 + desc.Columns);
		}
		break;
	}
	case D3D_SVC_MATRIX_COLUMNS:
	case D3D_SVC_MATRIX_ROWS:
		if (desc.Rows > 2 && desc.Columns > 2)
		{
			UINT index = (desc.Rows - 2) * 3 + (desc.Columns - 2);
			return UniformType(UT_MATRIX_2X2 + index);
		}
	case D3D_SVC_STRUCT:
		return UT_STRUCT;
	default:
		return UT_UNKNOWN;
	}
	return UT_UNKNOWN;
}

CU_NS_END