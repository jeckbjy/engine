#include "D3D12_Mapping.h"
#include "D3D12_Program.h"

CUTE_NS_BEGIN

DXGI_FORMAT D3D12_Mapping::getFormat(RESOURCE_FORMAT format)
{
	static const DXGI_FORMAT table[] = 
	{
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_R32G32B32A32_UINT,
		DXGI_FORMAT_R32G32B32A32_SINT,
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		DXGI_FORMAT_R16G16B16A16_UINT,
		DXGI_FORMAT_R16G16B16A16_SINT,
		DXGI_FORMAT_R32G32B32_FLOAT,
		DXGI_FORMAT_R32G32B32_UINT,
		DXGI_FORMAT_R32G32B32_SINT,
		DXGI_FORMAT_R32G32_FLOAT,
		DXGI_FORMAT_R32G32_UINT,
		DXGI_FORMAT_R32G32_SINT,
		DXGI_FORMAT_R16G16_FLOAT,
		DXGI_FORMAT_R16G16_UINT,
		DXGI_FORMAT_R16G16_SINT,
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_R32_UINT,
		DXGI_FORMAT_R32_SINT,
		DXGI_FORMAT_R16_FLOAT,
		DXGI_FORMAT_R16_UINT,
		DXGI_FORMAT_R16_SINT,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
		DXGI_FORMAT_B8G8R8A8_UNORM,
		DXGI_FORMAT_R8G8_UNORM,
		DXGI_FORMAT_R8_UNORM,
		DXGI_FORMAT_D32_FLOAT,
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		DXGI_FORMAT_D16_UNORM,
		DXGI_FORMAT_BC1_UNORM_SRGB,
		DXGI_FORMAT_BC1_UNORM,
		DXGI_FORMAT_BC2_UNORM_SRGB,
		DXGI_FORMAT_BC2_UNORM,
		DXGI_FORMAT_BC3_UNORM_SRGB,
		DXGI_FORMAT_BC3_UNORM,
		DXGI_FORMAT_BC4_UNORM,
		DXGI_FORMAT_BC4_SNORM,
		DXGI_FORMAT_BC5_UNORM,
		DXGI_FORMAT_BC5_SNORM,
		DXGI_FORMAT_BC6H_UF16,
		DXGI_FORMAT_BC6H_SF16,
		DXGI_FORMAT_BC7_UNORM_SRGB,
		DXGI_FORMAT_BC7_UNORM,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_R10G10B10A2_UNORM,
		DXGI_FORMAT_R10G10B10A2_UINT,
		DXGI_FORMAT_R11G11B10_FLOAT,
	};

	return table[format];
}

D3D12_RESOURCE_DIMENSION D3D12_Mapping::getTextureType(RESOURCE_DIMENSION type)
{
	//switch (type)
	//{
	//case TEX_1D: return D3D12_RESOURCE_DIMENSION_TEXTURE1D;
	//case TEX_2D: return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//case TEX_3D: return D3D12_RESOURCE_DIMENSION_TEXTURE3D;
	//}

	return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
}

D3D12_PRIMITIVE_TOPOLOGY D3D12_Mapping::getTopology(PRIMITIVE_TOPOLOGY topology)
{
	return D3D12_PRIMITIVE_TOPOLOGY(topology);
}

D3D12_PRIMITIVE_TOPOLOGY_TYPE D3D12_Mapping::getTopologyType(PRIMITIVE_TOPOLOGY topology)
{
	// todo:convert?
	return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
}

D3D12_PRIMITIVE_TOPOLOGY D3D12_Mapping::getPrimitive(PRIMITIVE_TOPOLOGY topology, uint32_t controlCount)
{
	if (topology != PRIMITIVE_TOPOLOGY_PATCHLIST)
	{
		static const D3D_PRIMITIVE_TOPOLOGY table[] = 
		{
			D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
			D3D_PRIMITIVE_TOPOLOGY_LINELIST,
			D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
			D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ,
			D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ,
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ,
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ,
		};

		return table[topology];
	}
	else
	{
		return (D3D_PRIMITIVE_TOPOLOGY)(D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST + controlCount);
	}
}

D3D12_FILL_MODE D3D12_Mapping::getFillMode(POLYGON_MODE mode)
{
	D3D12_FILL_MODE table[] = 
	{
		D3D12_FILL_MODE_SOLID,
		D3D12_FILL_MODE_WIREFRAME
	};

	return table[mode];
}

D3D12_CULL_MODE D3D12_Mapping::getCullMode(CULL_MODE mode)
{
	switch (mode)
	{
	case CULL_MODE_NONE:
		return D3D12_CULL_MODE_NONE;
	case CULL_MODE_FRONT:
		return D3D12_CULL_MODE_FRONT;
	case CULL_MODE_BACK:
		return D3D12_CULL_MODE_BACK;
	default:
		return D3D12_CULL_MODE_BACK;
	}
}

D3D12_LOGIC_OP D3D12_Mapping::getLogicOp(LOGIC_OP op)
{
	D3D12_LOGIC_OP table[] = 
	{
		D3D12_LOGIC_OP_CLEAR,
		D3D12_LOGIC_OP_SET,
		D3D12_LOGIC_OP_COPY,
		D3D12_LOGIC_OP_COPY_INVERTED,
		D3D12_LOGIC_OP_NOOP,
		D3D12_LOGIC_OP_INVERT,
		D3D12_LOGIC_OP_AND,
		D3D12_LOGIC_OP_NAND,
		D3D12_LOGIC_OP_OR,
		D3D12_LOGIC_OP_NOR,
		D3D12_LOGIC_OP_XOR,
		D3D12_LOGIC_OP_EQUIV,
		D3D12_LOGIC_OP_AND_REVERSE,
		D3D12_LOGIC_OP_AND_INVERTED,
		D3D12_LOGIC_OP_OR_REVERSE,
		D3D12_LOGIC_OP_INVERT
	};

	return table[op];
}

D3D12_BLEND_OP D3D12_Mapping::getBlendOp(BLEND_OP op)
{
	D3D12_BLEND_OP table[] = 
	{
		D3D12_BLEND_OP_ADD,
		D3D12_BLEND_OP_SUBTRACT,
		D3D12_BLEND_OP_REV_SUBTRACT,
		D3D12_BLEND_OP_MIN,
		D3D12_BLEND_OP_MAX
	};

	return table[op];
}

D3D12_BLEND D3D12_Mapping::getBlendFactor(BLEND_FACTOR factor)
{
	D3D12_BLEND table[] = 
	{
		D3D12_BLEND_ZERO,
		D3D12_BLEND_ONE,
		D3D12_BLEND_SRC_COLOR,
		D3D12_BLEND_INV_SRC_COLOR,
		D3D12_BLEND_SRC_ALPHA,
		D3D12_BLEND_INV_SRC_ALPHA,
		D3D12_BLEND_DEST_ALPHA,
		D3D12_BLEND_INV_DEST_ALPHA,
		D3D12_BLEND_DEST_COLOR,
		D3D12_BLEND_INV_DEST_COLOR,
		D3D12_BLEND_SRC_ALPHA_SAT,
		D3D12_BLEND_SRC1_COLOR,
		D3D12_BLEND_INV_SRC1_COLOR,
		D3D12_BLEND_SRC1_ALPHA,
		D3D12_BLEND_INV_SRC1_ALPHA
	};

	return table[factor];
}

D3D12_STENCIL_OP D3D12_Mapping::getStencilOp(STENCIL_OP op)
{
	D3D12_STENCIL_OP table[] = 
	{
		D3D12_STENCIL_OP_KEEP,
		D3D12_STENCIL_OP_ZERO,
		D3D12_STENCIL_OP_REPLACE,
		D3D12_STENCIL_OP_INCR_SAT,
		D3D12_STENCIL_OP_DECR_SAT,
		D3D12_STENCIL_OP_INVERT,
		D3D12_STENCIL_OP_INCR,
		D3D12_STENCIL_OP_DECR
	};

	return table[op];
}

D3D12_COMPARISON_FUNC D3D12_Mapping::getCompareOp(COMPARE_OP op)
{
	static const D3D12_COMPARISON_FUNC table[] = 
	{
		D3D12_COMPARISON_FUNC_NEVER,
		D3D12_COMPARISON_FUNC_LESS,
		D3D12_COMPARISON_FUNC_EQUAL,
		D3D12_COMPARISON_FUNC_LESS_EQUAL,
		D3D12_COMPARISON_FUNC_GREATER,
		D3D12_COMPARISON_FUNC_GREATER_EQUAL,
		D3D12_COMPARISON_FUNC_ALWAYS,
	};

	return table[op];
}

D3D12_TEXTURE_ADDRESS_MODE D3D12_Mapping::getAddressMode(TEXTURE_ADDRESS_MODE mode)
{
	D3D12_TEXTURE_ADDRESS_MODE table[] = 
	{
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_BORDER,
		D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE
	};

	return table[mode];
}

D3D12_INPUT_CLASSIFICATION D3D12_Mapping::getInputClassification(INPUT_CLASSIFICATION classification)
{
	D3D12_INPUT_CLASSIFICATION table[] = 
	{
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA
	};

	return table[classification];
}

D3D12_COMMAND_LIST_TYPE D3D12_Mapping::getCommandListType(COMMANDLIST_TYPE type)
{
	static const D3D12_COMMAND_LIST_TYPE table[] = 
	{
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		D3D12_COMMAND_LIST_TYPE_BUNDLE,
		D3D12_COMMAND_LIST_TYPE_COMPUTE,
		D3D12_COMMAND_LIST_TYPE_COPY
	};

	return table[type];
}

D3D12_HEAP_TYPE D3D12_Mapping::getHeapType(HEAP_TYPE type)
{
	static const D3D12_HEAP_TYPE table[] = 
	{
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_HEAP_TYPE_UPLOAD,
		D3D12_HEAP_TYPE_READBACK
	};

	return table[type];
}

D3D12_CPU_PAGE_PROPERTY D3D12_Mapping::getCpuPageProperty(CPU_PAGE_PROPERTY prop)
{
	static const D3D12_CPU_PAGE_PROPERTY table[] = 
	{
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_CPU_PAGE_PROPERTY_NOT_AVAILABLE,
		D3D12_CPU_PAGE_PROPERTY_WRITE_COMBINE,
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
	};

	return table[prop];
}

D3D12_RESOURCE_FLAGS D3D12_Mapping::getResourceFlags(RESOURCE_USAGE usage)
{
	D3D12_RESOURCE_FLAGS result = D3D12_RESOURCE_FLAG_NONE;

	if (usage & RESOURCE_USAGE_COLOR_TARGET)
		result |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	if (usage & RESOURCE_USAGE_DEPTH_TARGET)
		result |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	if (usage & RESOURCE_USAGE_UNORDERD_ACCESS)
		result |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	return result;
}

D3D12_RESOURCE_STATES D3D12_Mapping::getResourceStates(RESOURCE_STATE state)
{
	static const D3D12_RESOURCE_STATES table[] =
	{
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		D3D12_RESOURCE_STATE_INDEX_BUFFER,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		D3D12_RESOURCE_STATE_DEPTH_READ,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_STREAM_OUT,
		D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT,
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_COPY_SOURCE,
		D3D12_RESOURCE_STATE_RESOLVE_DEST,
		D3D12_RESOURCE_STATE_RESOLVE_SOURCE,
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_PREDICATION
	};

	return table[state];
}

D3D12_RESOURCE_DIMENSION D3D12_Mapping::getResorceDimension(RESOURCE_DIMENSION dimension)
{
	static const D3D12_RESOURCE_DIMENSION table[] = 
	{
		D3D12_RESOURCE_DIMENSION_UNKNOWN,
		D3D12_RESOURCE_DIMENSION_BUFFER,
		D3D12_RESOURCE_DIMENSION_TEXTURE1D,
		D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		D3D12_RESOURCE_DIMENSION_TEXTURE3D,
		D3D12_RESOURCE_DIMENSION_TEXTURE2D
	};

	return table[dimension];
}

D3D12_FILTER D3D12_Mapping::getFilter(FILTER_MODE min, FILTER_MODE mag, MIPMAP_MODE mip, bool compare, bool anisotropy)
{
	D3D12_FILTER_REDUCTION_TYPE reduction = compare ? D3D12_FILTER_REDUCTION_TYPE_COMPARISON : D3D12_FILTER_REDUCTION_TYPE_STANDARD;
	if (anisotropy)
		return D3D12_ENCODE_ANISOTROPIC_FILTER(reduction);
	else
		return D3D12_ENCODE_BASIC_FILTER(min, mag, mip, reduction);
}

void D3D12_Mapping::getBorderColor(BORDER_COLOR color, FLOAT* result)
{
	switch (color)
	{
	case BORDER_COLOR_TRANSPARENT_BLACK:
	{
		result[0] = 0.0f;
		result[1] = 0.0f;
		result[2] = 0.0f;
		result[3] = 0.0f;
	}
	break;
	case BORDER_COLOR_OPAQUE_BLACK:
	{
		result[0] = 0.0f;
		result[1] = 0.0f;
		result[2] = 0.0f;
		result[3] = 1.0f;
	}
	break;
	case BORDER_COLOR_OPAQUE_WHITE:
	{
		result[0] = 1.0f;
		result[1] = 1.0f;
		result[2] = 1.0f;
		result[3] = 1.0f;
	}
	break;
	}
}

//void D3D12_Mapping::fillShader(D3D12_SHADER_BYTECODE& code, ShaderStage* prog)
//{
//	D3D12_Shader* shader = (D3D12_Shader*)prog;
//	if (shader)
//	{
//		ID3DBlob* blob = shader->getCode();
//		code.pShaderBytecode = blob->GetBufferPointer();
//		code.BytecodeLength = blob->GetBufferSize();
//	}
//	else
//	{
//		code.pShaderBytecode = NULL;
//		code.BytecodeLength = 0;
//	}
//}
//
//void D3D12_Mapping::fillRasterizerState(D3D12_RASTERIZER_DESC& state, const RasterizerDesc& desc)
//{
//	state.FillMode = getFillMode(desc.fillMode);
//	state.CullMode = getCullMode(desc.cullMode);
//	state.FrontCounterClockwise = (desc.frontFace == FRONT_FACE_COUNTER_CLOCKWISE);
//	state.DepthBias = (INT)desc.depthBiasConstantFactor;
//	state.DepthBiasClamp = desc.depthBiasClamp;
//	state.DepthClipEnable = desc.depthClampEnable;
//	// 如何对应??
//	state.MultisampleEnable = FALSE;
//	state.AntialiasedLineEnable = FALSE;
//	state.ForcedSampleCount = 0;
//	state.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
//}
//
//void D3D12_Mapping::fillBlendState(D3D12_BLEND_DESC& state, const BlendDesc& desc)
//{
//	BOOL logicOpEnable = desc.logicOpEnable;
//	D3D12_LOGIC_OP logicOp = getLogicOp(desc.logicOp);
//	// target
//	for (int i = 0; i < 8; ++i)
//	{
//		D3D12_RENDER_TARGET_BLEND_DESC& target = state.RenderTarget[i];
//		target.LogicOpEnable = logicOpEnable;
//		target.LogicOp = logicOp;
//		fillBlendTarget(target, desc.targets[i]);
//	}
//}
//
//void D3D12_Mapping::fillBlendTarget(D3D12_RENDER_TARGET_BLEND_DESC& state, const BlendTargetDesc& desc)
//{
//	state.SrcBlend = getBlendFactor(desc.srcColorFactor);
//	state.DestBlend = getBlendFactor(desc.dstColorFactor);
//	state.BlendOp = getBlendOp(desc.colorOp);
//	state.SrcBlendAlpha = getBlendFactor(desc.srcAlphaFactor);
//	state.DestBlendAlpha = getBlendFactor(desc.dstAlphaFactor);
//	state.BlendOpAlpha = getBlendOp(desc.alphaOp);
//	state.RenderTargetWriteMask = (UINT8)desc.colorWriteMask;
//}
//
//void D3D12_Mapping::fillDepthStencilState(D3D12_DEPTH_STENCIL_DESC& state, const DepthStencilDesc& desc)
//{
//	state.DepthEnable = desc.depthTestEnable;
//	state.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
//	state.DepthFunc = getCompareOp(desc.depthCompareOp);
//	state.StencilEnable = desc.stencilTestEnable;
//	state.StencilReadMask = 0;
//	state.StencilWriteMask = 0;
//	fillStencilOpState(state.FrontFace, desc.front);
//	fillStencilOpState(state.BackFace, desc.back);
//}
//
//void D3D12_Mapping::fillStencilOpState(D3D12_DEPTH_STENCILOP_DESC& state, const StencilOpDesc& desc)
//{
//	state.StencilFailOp = getStencilOp(desc.failOp);
//	state.StencilDepthFailOp = getStencilOp(desc.depthFailOp);
//	state.StencilPassOp = getStencilOp(desc.passOp);
//	state.StencilFunc = getCompareOp(desc.compareOp);
//	// ignore others??
//}
//
//void D3D12_Mapping::fillSamplerDesc(D3D12_SAMPLER_DESC& info, const SamplerDesc& desc)
//{
//	// 目前只支持standard,且filter完全对应
//	D3D12_FILTER_REDUCTION_TYPE reduction = D3D12_FILTER_REDUCTION_TYPE_STANDARD;
//	if (desc.anisotropyEnable)
//		info.Filter = D3D12_ENCODE_ANISOTROPIC_FILTER(reduction);
//	else
//		info.Filter = D3D12_ENCODE_BASIC_FILTER(desc.minFilter, desc.magFilter, desc.mipFilter, reduction);
//	info.AddressU = getAddressMode(desc.addrU);
//	info.AddressV = getAddressMode(desc.addrV);
//	info.AddressW = getAddressMode(desc.addrW);
//	info.MipLODBias = desc.mipLodBias;
//	info.MaxAnisotropy = (UINT)desc.maxAnisotropy;
//	info.ComparisonFunc = getCompareOp(desc.compareOp);
//	info.MinLOD = desc.minLod;
//	info.MaxLOD = desc.maxLod;
//	memcpy(info.BorderColor, desc.borderColor, sizeof(info.BorderColor));
//}
//
//void D3D12_Mapping::fillSemantic(Semantic semantic, LPCSTR& name, UINT& index)
//{
//	static const LPCSTR sematic_names[] = { "POSITION", "NORMAL", "COLOR", "TANGENT", "BINORMAL", "BLENDWEIGHTS", "BLENDINDICES" };
//	if (semantic >= SEMANTIC_TEXCOORD0)
//	{
//		name = "TEXCOORD";
//		index = semantic - SEMANTIC_TEXCOORD0;
//	}
//	else
//	{
//		name = sematic_names[semantic];
//		index = 0;
//	}
//}

CUTE_NS_END
