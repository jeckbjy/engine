#include "D3D11_Mapping.h"

CUTE_NS_BEGIN

DXGI_FORMAT D3D11_Mapping::getFormat(PixelFormat format)
{
	switch (format)
	{
	case PF_UNKNOWN:				return DXGI_FORMAT_UNKNOWN;
	case PF_R4G4_UNORM:				return DXGI_FORMAT_UNKNOWN;
	case PF_R4G4B4A4_UNORM:			return DXGI_FORMAT_UNKNOWN;
	case PF_B4G4R4A4_UNORM:			return DXGI_FORMAT_B4G4R4A4_UNORM;
	case PF_R5G6B5_UNORM:			return DXGI_FORMAT_UNKNOWN;
	case PF_B5G6R5_UNORM:			return DXGI_FORMAT_B5G6R5_UNORM;
	case PF_R5G5B5A1_UNORM:			return DXGI_FORMAT_UNKNOWN;
	case PF_B5G5R5A1_UNORM:			return DXGI_FORMAT_B5G5R5A1_UNORM;
	case PF_A1R5G5B5_UNORM:			return DXGI_FORMAT_UNKNOWN;
	case PF_R8_UNORM:				return DXGI_FORMAT_R8_UNORM;
	case PF_R8_SNORM:				return DXGI_FORMAT_R8_SNORM;
	case PF_R8_USCALED:				return DXGI_FORMAT_UNKNOWN;
	case PF_R8_SSCALED:				return DXGI_FORMAT_UNKNOWN;
	case PF_R8_UINT:				return DXGI_FORMAT_R8_UINT;
	case PF_R8_SINT:				return DXGI_FORMAT_R8_SINT;
	case PF_R8_SRGB:				return DXGI_FORMAT_UNKNOWN;
	case PF_R8G8_UNORM:				return DXGI_FORMAT_R8G8_UNORM;
	case PF_R8G8_SNORM:				return DXGI_FORMAT_R8G8_SNORM;
	case PF_R8G8_USCALED:			return DXGI_FORMAT_UNKNOWN;
	case PF_R8G8_SSCALED:			return DXGI_FORMAT_UNKNOWN;
	case PF_R8G8_UINT:				return DXGI_FORMAT_R8G8_UINT;
	case PF_R8G8_SINT:				return DXGI_FORMAT_R8G8_SINT;
	case PF_R8G8_SRGB:				return DXGI_FORMAT_UNKNOWN;
	case PF_R8G8B8_UNORM:			return DXGI_FORMAT_UNKNOWN;
	case PF_R8G8B8_SNORM:			return DXGI_FORMAT_UNKNOWN;
	case PF_R8G8B8_USCALED:			return DXGI_FORMAT_UNKNOWN;
	case PF_R8G8B8_SSCALED:			return DXGI_FORMAT_UNKNOWN;
	case PF_R8G8B8_UINT:			return DXGI_FORMAT_UNKNOWN;
	case PF_R8G8B8_SINT:			return DXGI_FORMAT_UNKNOWN;
	case PF_R8G8B8_SRGB:			return DXGI_FORMAT_UNKNOWN;
	case PF_B8G8R8_UNORM:			return DXGI_FORMAT_UNKNOWN;
	case PF_B8G8R8_SNORM:			return DXGI_FORMAT_UNKNOWN;
	case PF_B8G8R8_USCALED:			return DXGI_FORMAT_UNKNOWN;
	case PF_B8G8R8_SSCALED:			return DXGI_FORMAT_UNKNOWN;
	case PF_B8G8R8_UINT:			return DXGI_FORMAT_UNKNOWN;
	case PF_B8G8R8_SINT:			return DXGI_FORMAT_UNKNOWN;
	case PF_B8G8R8_SRGB:			return DXGI_FORMAT_UNKNOWN;
	case PF_R8G8B8A8_UNORM:			return DXGI_FORMAT_R8G8B8A8_UNORM;
	case PF_R8G8B8A8_SNORM:			return DXGI_FORMAT_R8G8B8A8_SNORM;
	case PF_R8G8B8A8_USCALED:		return DXGI_FORMAT_UNKNOWN;
	case PF_R8G8B8A8_SSCALED:		return DXGI_FORMAT_UNKNOWN;
	case PF_R8G8B8A8_UINT:			return DXGI_FORMAT_R8G8B8A8_UINT;
	case PF_R8G8B8A8_SINT:			return DXGI_FORMAT_R8G8B8A8_SINT;
	case PF_R8G8B8A8_SRGB:			return DXGI_FORMAT_UNKNOWN;
	case PF_B8G8R8A8_UNORM:			return DXGI_FORMAT_B8G8R8A8_UNORM;
	case PF_B8G8R8A8_SNORM:			return DXGI_FORMAT_UNKNOWN;
	case PF_B8G8R8A8_USCALED:		return DXGI_FORMAT_UNKNOWN;
	case PF_B8G8R8A8_SSCALED:		return DXGI_FORMAT_UNKNOWN; 
	case PF_B8G8R8A8_UINT:			return DXGI_FORMAT_UNKNOWN;
	case PF_B8G8R8A8_SINT:			return DXGI_FORMAT_UNKNOWN;
	case PF_B8G8R8A8_SRGB:			return DXGI_FORMAT_UNKNOWN;
	case PF_A8B8G8R8_UNORM:			return DXGI_FORMAT_UNKNOWN;
	case PF_A8B8G8R8_SNORM:			return DXGI_FORMAT_UNKNOWN;
	case PF_A8B8G8R8_USCALED:		return DXGI_FORMAT_UNKNOWN;
	case PF_A8B8G8R8_SSCALED:		return DXGI_FORMAT_UNKNOWN;
	case PF_A8B8G8R8_UINT:			return DXGI_FORMAT_UNKNOWN;
	case PF_A8B8G8R8_SINT:			return DXGI_FORMAT_UNKNOWN;
	case PF_A8B8G8R8_SRGB:			return DXGI_FORMAT_UNKNOWN;
	case PF_A2R10G10B10_UNORM:		return DXGI_FORMAT_UNKNOWN;
	case PF_A2R10G10B10_SNORM:		return DXGI_FORMAT_UNKNOWN;
	case PF_A2R10G10B10_USCALED:	return DXGI_FORMAT_UNKNOWN;
	case PF_A2R10G10B10_SSCALED:	return DXGI_FORMAT_UNKNOWN;
	case PF_A2R10G10B10_UINT:		return DXGI_FORMAT_UNKNOWN;
	case PF_A2R10G10B10_SINT:		return DXGI_FORMAT_UNKNOWN;
	case PF_A2B10G10R10_UNORM:		return DXGI_FORMAT_UNKNOWN;
	case PF_A2B10G10R10_SNORM:		return DXGI_FORMAT_UNKNOWN;
	case PF_A2B10G10R10_USCALED:	return DXGI_FORMAT_UNKNOWN;
	case PF_A2B10G10R10_SSCALED:	return DXGI_FORMAT_UNKNOWN;
	case PF_A2B10G10R10_UINT:		return DXGI_FORMAT_UNKNOWN;
	case PF_A2B10G10R10_SINT:		return DXGI_FORMAT_UNKNOWN;
	case PF_R16_UNORM:				return DXGI_FORMAT_R16_UNORM;
	case PF_R16_SNORM:				return DXGI_FORMAT_R16_SNORM;
	case PF_R16_USCALED:			return DXGI_FORMAT_UNKNOWN;
	case PF_R16_SSCALED:			return DXGI_FORMAT_UNKNOWN;
	case PF_R16_UINT:				return DXGI_FORMAT_R16_UINT;
	case PF_R16_SINT:				return DXGI_FORMAT_R16_SINT;
	case PF_R16_SFLOAT:				return DXGI_FORMAT_R16_FLOAT;
	case PF_R16G16_UNORM:			return DXGI_FORMAT_R16G16_UNORM;
	case PF_R16G16_SNORM:			return DXGI_FORMAT_R16G16_SNORM;
	case PF_R16G16_USCALED:			return DXGI_FORMAT_UNKNOWN;
	case PF_R16G16_SSCALED:			return DXGI_FORMAT_UNKNOWN;
	case PF_R16G16_UINT:			return DXGI_FORMAT_R16G16_UINT;
	case PF_R16G16_SINT:			return DXGI_FORMAT_R16G16_SINT;
	case PF_R16G16_SFLOAT:			return DXGI_FORMAT_R16G16_FLOAT;
	case PF_R16G16B16_UNORM:		return DXGI_FORMAT_UNKNOWN;
	case PF_R16G16B16_SNORM:		return DXGI_FORMAT_UNKNOWN;
	case PF_R16G16B16_USCALED:		return DXGI_FORMAT_UNKNOWN;
	case PF_R16G16B16_SSCALED:		return DXGI_FORMAT_UNKNOWN;
	case PF_R16G16B16_UINT:			return DXGI_FORMAT_UNKNOWN;
	case PF_R16G16B16_SINT:			return DXGI_FORMAT_UNKNOWN;
	case PF_R16G16B16_SFLOAT:		return DXGI_FORMAT_UNKNOWN;
	case PF_R16G16B16A16_UNORM:		return DXGI_FORMAT_R16G16B16A16_UNORM;
	case PF_R16G16B16A16_SNORM:		return DXGI_FORMAT_R16G16B16A16_SNORM;
	case PF_R16G16B16A16_USCALED:	return DXGI_FORMAT_UNKNOWN;
	case PF_R16G16B16A16_SSCALED:	return DXGI_FORMAT_UNKNOWN;
	case PF_R16G16B16A16_UINT:		return DXGI_FORMAT_R16G16B16A16_UINT;
	case PF_R16G16B16A16_SINT:		return DXGI_FORMAT_R16G16B16A16_SINT;
	case PF_R16G16B16A16_SFLOAT:	return DXGI_FORMAT_R16G16B16A16_FLOAT;
	case PF_R32_UINT:				return DXGI_FORMAT_R32_UINT;
	case PF_R32_SINT:				return DXGI_FORMAT_R32_SINT;
	case PF_R32_SFLOAT:				return DXGI_FORMAT_R32_FLOAT;
	case PF_R32G32_UINT:			return DXGI_FORMAT_R32G32_UINT;
	case PF_R32G32_SINT:			return DXGI_FORMAT_R32G32_SINT;
	case PF_R32G32_SFLOAT:			return DXGI_FORMAT_R32G32_FLOAT;
	case PF_R32G32B32_UINT:			return DXGI_FORMAT_R32G32B32_UINT;
	case PF_R32G32B32_SINT:			return DXGI_FORMAT_R32G32B32_SINT;
	case PF_R32G32B32_SFLOAT:		return DXGI_FORMAT_R32G32B32_FLOAT;
	case PF_R32G32B32A32_UINT:		return DXGI_FORMAT_R32G32B32A32_UINT;
	case PF_R32G32B32A32_SINT:		return DXGI_FORMAT_R32G32B32A32_SINT;
	case PF_R32G32B32A32_SFLOAT:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case PF_R64_UINT:				return DXGI_FORMAT_UNKNOWN;
	case PF_R64_SINT:				return DXGI_FORMAT_UNKNOWN;
	case PF_R64_SFLOAT:				return DXGI_FORMAT_UNKNOWN;
	case PF_R64G64_UINT:			return DXGI_FORMAT_UNKNOWN;
	case PF_R64G64_SINT:			return DXGI_FORMAT_UNKNOWN;
	case PF_R64G64_SFLOAT:			return DXGI_FORMAT_UNKNOWN;
	case PF_R64G64B64_UINT:			return DXGI_FORMAT_UNKNOWN;
	case PF_R64G64B64_SINT:			return DXGI_FORMAT_UNKNOWN;
	case PF_R64G64B64_SFLOAT:		return DXGI_FORMAT_UNKNOWN;
	case PF_R64G64B64A64_UINT:		return DXGI_FORMAT_UNKNOWN;
	case PF_R64G64B64A64_SINT:		return DXGI_FORMAT_UNKNOWN;
	case PF_R64G64B64A64_SFLOAT:	return DXGI_FORMAT_UNKNOWN;
	case PF_B10G11R11_UFLOAT:		return DXGI_FORMAT_UNKNOWN;
	case PF_E5B9G9R9_UFLOAT:		return DXGI_FORMAT_UNKNOWN;
	case PF_D16_UNORM:				return DXGI_FORMAT_D16_UNORM;
	case PF_X8_D24_UNORM:			return DXGI_FORMAT_D24_UNORM_S8_UINT;
	case PF_D32_SFLOAT:				return DXGI_FORMAT_D32_FLOAT;
	case PF_S8_UINT:				return DXGI_FORMAT_UNKNOWN;
	case PF_D16_UNORM_S8_UINT:		return DXGI_FORMAT_D16_UNORM;
	case PF_D24_UNORM_S8_UINT:		return DXGI_FORMAT_D24_UNORM_S8_UINT;
	case PF_D32_SFLOAT_S8_UINT:		return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	case PF_BC1_RGB_UNORM:			return DXGI_FORMAT_BC1_UNORM;
	case PF_BC1_RGB_SRGB:			return DXGI_FORMAT_BC1_UNORM_SRGB;
	case PF_BC1_RGBA_UNORM:			return DXGI_FORMAT_BC1_UNORM;
	case PF_BC1_RGBA_SRGB:			return DXGI_FORMAT_BC1_UNORM_SRGB;
	case PF_BC2_UNORM:				return DXGI_FORMAT_BC2_UNORM;
	case PF_BC2_SRGB:				return DXGI_FORMAT_BC2_UNORM_SRGB;
	case PF_BC3_UNORM:				return DXGI_FORMAT_BC3_UNORM;
	case PF_BC3_SRGB:				return DXGI_FORMAT_BC3_UNORM_SRGB;
	case PF_BC4_UNORM:				return DXGI_FORMAT_BC4_UNORM;
	case PF_BC4_SNORM:				return DXGI_FORMAT_BC4_SNORM;
	case PF_BC5_UNORM:				return DXGI_FORMAT_BC5_UNORM;
	case PF_BC5_SNORM:				return DXGI_FORMAT_BC5_SNORM;
	case PF_BC6H_UFLOAT:			return DXGI_FORMAT_BC6H_UF16;
	case PF_BC6H_SFLOAT:			return DXGI_FORMAT_BC6H_SF16;
	case PF_BC7_UNORM:				return DXGI_FORMAT_BC7_UNORM;
	case PF_BC7_SRGB:				return DXGI_FORMAT_BC7_UNORM_SRGB;
	case PF_ETC2_R8G8B8_UNORM:		return DXGI_FORMAT_UNKNOWN;
	case PF_ETC2_R8G8B8_SRGB:		return DXGI_FORMAT_UNKNOWN;
	case PF_ETC2_R8G8B8A1_UNORM:	return DXGI_FORMAT_UNKNOWN;
	case PF_ETC2_R8G8B8A1_SRGB:		return DXGI_FORMAT_UNKNOWN;
	case PF_ETC2_R8G8B8A8_UNORM:	return DXGI_FORMAT_UNKNOWN;
	case PF_ETC2_R8G8B8A8_SRGB:		return DXGI_FORMAT_UNKNOWN;
	case PF_EAC_R11_UNORM:			return DXGI_FORMAT_UNKNOWN;
	case PF_EAC_R11_SNORM:			return DXGI_FORMAT_UNKNOWN;
	case PF_EAC_R11G11_UNORM:		return DXGI_FORMAT_UNKNOWN;
	case PF_EAC_R11G11_SNORM:		return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_4x4_UNORM:			return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_4x4_SRGB:			return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_5x4_UNORM:			return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_5x4_SRGB:			return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_5x5_UNORM:			return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_5x5_SRGB:			return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_6x5_UNORM:			return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_6x5_SRGB:			return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_6x6_UNORM:			return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_6x6_SRGB:			return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_8x5_UNORM:			return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_8x5_SRGB:			return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_8x6_UNORM:			return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_8x6_SRGB:			return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_8x8_UNORM:			return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_8x8_SRGB:			return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_10x5_UNORM:		return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_10x5_SRGB:			return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_10x6_UNORM:		return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_10x6_SRGB:			return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_10x8_UNORM:		return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_10x8_SRGB:			return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_10x10_UNORM:		return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_10x10_SRGB:		return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_12x10_UNORM:		return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_12x10_SRGB:		return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_12x12_UNORM:		return DXGI_FORMAT_UNKNOWN;
	case PF_ASTC_12x12_SRGB:		return DXGI_FORMAT_UNKNOWN;
	default:
		return DXGI_FORMAT_UNKNOWN;
	}
}

D3D11_PRIMITIVE_TOPOLOGY D3D11_Mapping::getTopoloty(Topology topology)
{
	return D3D11_PRIMITIVE_TOPOLOGY(topology);
}

D3D11_FILL_MODE D3D11_Mapping::getFillMode(FillMode mode)
{
	switch (mode)
	{
	case FILL_MODE_SOLID:
		return D3D11_FILL_SOLID;
	case FILL_MODE_LINE:
	case FILL_MODE_POINT:
		return D3D11_FILL_WIREFRAME;
	default:
		return D3D11_FILL_SOLID;
	}
}

D3D11_CULL_MODE D3D11_Mapping::getCullMode(CullMode mode)
{
	switch (mode)
	{
	case CULL_MODE_NONE:
		return D3D11_CULL_NONE;
	case CULL_MODE_FRONT:
		return D3D11_CULL_FRONT;
	case CULL_MODE_BACK:
		return D3D11_CULL_BACK;
	case CULL_MODE_BOTH:// not support
		return D3D11_CULL_BACK;
	default:
		return D3D11_CULL_BACK;
	}
}

D3D11_BLEND D3D11_Mapping::getBlendFactor(BlendFactor factor)
{
	switch (factor)
	{
	case BLEND_FACTOR_ZERO:
		return D3D11_BLEND_ZERO;
	case BLEND_FACTOR_ONE:
		return D3D11_BLEND_ONE;
	case BLEND_FACTOR_SRC_COLOR:
		return D3D11_BLEND_SRC_COLOR;
	case BLEND_FACTOR_ONE_MINUS_SRC_COLOR:
		return D3D11_BLEND_INV_SRC_COLOR;
	case BLEND_FACTOR_SRC_ALPHA:
		return D3D11_BLEND_SRC_ALPHA;
	case BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:
		return D3D11_BLEND_INV_SRC_ALPHA;
	case BLEND_FACTOR_DST_COLOR:
		return D3D11_BLEND_DEST_COLOR;
	case BLEND_FACTOR_ONE_MINUS_DST_COLOR:
		return D3D11_BLEND_INV_DEST_COLOR;
	case BLEND_FACTOR_DST_ALPHA:
		return D3D11_BLEND_DEST_ALPHA;
	case BLEND_FACTOR_ONE_MINUS_DST_ALPHA:
		return D3D11_BLEND_INV_DEST_ALPHA;
	case BLEND_FACTOR_CONST_COLOR:
		return D3D11_BLEND_BLEND_FACTOR;
	case BLEND_FACTOR_ONE_MINUS_CONST_COLOR:
		return D3D11_BLEND_INV_BLEND_FACTOR;
	case BLEND_FACTOR_CONST_ALPHA:				// 不支持？？
		return D3D11_BLEND_BLEND_FACTOR;
	case BLEND_FACTOR_ONE_MINUS_CONST_ALPHA:	// 不支持？？
		return D3D11_BLEND_INV_BLEND_FACTOR;
	case BLEND_FACTOR_SRC_ALPHA_SATURATE:
		return D3D11_BLEND_SRC_ALPHA_SAT;
	case BLEND_FACTOR_SRC1_COLOR:
		return D3D11_BLEND_SRC1_COLOR;
	case BLEND_FACTOR_ONE_MINUS_SRC1_COLOR:
		return D3D11_BLEND_INV_SRC1_COLOR;
	case BLEND_FACTOR_SRC1_ALPHA:
		return D3D11_BLEND_SRC1_ALPHA;
	case BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA:
		return D3D11_BLEND_INV_SRC1_ALPHA;
	default:
		return D3D11_BLEND_ZERO;
	}
}

D3D11_BLEND_OP D3D11_Mapping::getBlendOp(BlendOp op)
{
	return (D3D11_BLEND_OP)op;
}

D3D11_LOGIC_OP D3D11_Mapping::getLogicOp(LogicOp op)
{
	switch (op)
	{
	case LOGIC_OP_CLEAR:
		return D3D11_LOGIC_OP_CLEAR;
	case LOGIC_OP_AND:
		return D3D11_LOGIC_OP_AND;
	case LOGIC_OP_AND_REVERSE:
		return D3D11_LOGIC_OP_AND_REVERSE;
	case LOGIC_OP_COPY:
		return D3D11_LOGIC_OP_COPY;
	case LOGIC_OP_AND_INVERTED:
		return D3D11_LOGIC_OP_AND_INVERTED;
	case LOGIC_OP_NO_OP:
		return D3D11_LOGIC_OP_NOOP;
	case LOGIC_OP_XOR:
		return D3D11_LOGIC_OP_XOR;
	case LOGIC_OP_OR:
		return D3D11_LOGIC_OP_OR;
	case LOGIC_OP_NOR:
		return D3D11_LOGIC_OP_NOR;
	case LOGIC_OP_EQUIV:
		return D3D11_LOGIC_OP_EQUIV;
	case LOGIC_OP_INVERT:
		return D3D11_LOGIC_OP_INVERT;
	case LOGIC_OP_OR_REVERSE:
		return D3D11_LOGIC_OP_OR_REVERSE;
	case LOGIC_OP_COPY_INVERTED:
		return D3D11_LOGIC_OP_COPY_INVERTED;
	case LOGIC_OP_OR_INVERTED:
		return D3D11_LOGIC_OP_OR_INVERTED;
	case LOGIC_OP_NAND:
		return D3D11_LOGIC_OP_NAND;
	case LOGIC_OP_SET:
		return D3D11_LOGIC_OP_SET;
	default:
		return D3D11_LOGIC_OP_CLEAR;
	}
}

D3D11_STENCIL_OP D3D11_Mapping::getStencilOp(StencilOp op)
{
	return (D3D11_STENCIL_OP)op;
}

D3D11_COMPARISON_FUNC D3D11_Mapping::getCompareOp(CompareOp op)
{
	return (D3D11_COMPARISON_FUNC)op;
}

D3D11_TEXTURE_ADDRESS_MODE D3D11_Mapping::getAddressMode(AddressMode mode)
{
	return (D3D11_TEXTURE_ADDRESS_MODE)mode;
}

DXGI_MODE_SCANLINE_ORDER D3D11_Mapping::getScanlineOrder(ScanlineOrderMode mode)
{
	return (DXGI_MODE_SCANLINE_ORDER)mode;
}

DXGI_MODE_SCALING D3D11_Mapping::getScaling(ScalingMode mode)
{
	return (DXGI_MODE_SCALING)mode;
}

DXGI_SWAP_EFFECT D3D11_Mapping::getSwapEffect(SwapMode mode)
{
	return (DXGI_SWAP_EFFECT)mode;
}

void D3D11_Mapping::fillSemantic(Semantic semantic, LPCSTR& name, UINT& index)
{
	static const LPCSTR sematic_names[] = { "POSITION", "NORMAL", "COLOR", "TANGENT", "BINORMAL", "BLENDWEIGHTS", "BLENDINDICES" };
	if (semantic >= SEMANTIC_TEXCOORD0)
	{
		name = "TEXCOORD";
		index = semantic - SEMANTIC_TEXCOORD0;
	}
	else
	{
		name = sematic_names[semantic];
		index = 0;
	}
}

void D3D11_Mapping::fillSampler(D3D11_SAMPLER_DESC& info, const SamplerDesc& desc)
{
	// 目前只支持standard,且filter完全对应
	D3D11_FILTER_REDUCTION_TYPE reduction = D3D11_FILTER_REDUCTION_TYPE_STANDARD;
	if (desc.anisotropyEnable)
		info.Filter = D3D11_ENCODE_ANISOTROPIC_FILTER(reduction);
	else
		info.Filter = D3D11_ENCODE_BASIC_FILTER(desc.minFilter, desc.magFilter, desc.mipFilter, reduction);

	// fill
	info.AddressU = getAddressMode(desc.addrU);
	info.AddressU = getAddressMode(desc.addrU);
	info.AddressV = getAddressMode(desc.addrV);
	info.AddressW = getAddressMode(desc.addrW);
	info.MipLODBias = desc.mipLodBias;
	info.MaxAnisotropy = (UINT)desc.maxAnisotropy;
	info.ComparisonFunc = getCompareOp(desc.compareOp);
	info.MinLOD = desc.minLod;
	info.MaxLOD = desc.maxLod;
	memcpy(info.BorderColor, desc.borderColor, sizeof(info.BorderColor));
}

void D3D11_Mapping::fillRasterizer(D3D11_RASTERIZER_DESC& info, const RasterizerDesc& desc)
{
	info.FillMode = getFillMode(desc.fillMode);
	info.CullMode = getCullMode(desc.cullMode);
	info.FrontCounterClockwise = (desc.frontFace == FRONT_FACE_COUNTER_CLOCKWISE);
	info.DepthBias = (INT)desc.depthBiasConstantFactor;
	info.DepthBiasClamp = desc.depthBiasClamp;
	info.DepthClipEnable = desc.depthClampEnable;
	// 如何对应??
	info.MultisampleEnable = FALSE;
	info.AntialiasedLineEnable = FALSE;
	info.ScissorEnable = FALSE;
}

void D3D11_Mapping::fillBlend(D3D11_BLEND_DESC& info, const BlendDesc& desc)
{
	//BOOL logicOpEnable = desc.logicOpEnable;
	//D3D11_LOGIC_OP logicOp = getLogicOp(desc.logicOp);
	// target
	for (int i = 0; i < 8; ++i)
	{
		D3D11_RENDER_TARGET_BLEND_DESC& target = info.RenderTarget[i];
		fillBlendTarget(target, desc.targets[i]);
	}
}

void D3D11_Mapping::fillBlendTarget(D3D11_RENDER_TARGET_BLEND_DESC& info, const BlendTargetDesc& desc)
{
	info.SrcBlend = getBlendFactor(desc.srcColorFactor);
	info.DestBlend = getBlendFactor(desc.dstColorFactor);
	info.BlendOp = getBlendOp(desc.colorOp);
	info.SrcBlendAlpha = getBlendFactor(desc.srcAlphaFactor);
	info.DestBlendAlpha = getBlendFactor(desc.dstAlphaFactor);
	info.BlendOpAlpha = getBlendOp(desc.alphaOp);
	info.RenderTargetWriteMask = (UINT8)desc.colorWriteMask;
}

void D3D11_Mapping::fillDepthStencil(D3D11_DEPTH_STENCIL_DESC& info, const DepthStencilDesc& desc)
{
	info.DepthEnable = desc.depthTestEnable;
	info.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	info.DepthFunc = getCompareOp(desc.depthCompareOp);
	info.StencilEnable = desc.stencilTestEnable;
	info.StencilReadMask = 0;
	info.StencilWriteMask = 0;
	fillDepthStencilOp(info.FrontFace, desc.front);
	fillDepthStencilOp(info.BackFace, desc.back);
}

void D3D11_Mapping::fillDepthStencilOp(D3D11_DEPTH_STENCILOP_DESC& info, const StencilOpDesc& desc)
{
	info.StencilFailOp = getStencilOp(desc.failOp);
	info.StencilDepthFailOp = getStencilOp(desc.depthFailOp);
	info.StencilPassOp = getStencilOp(desc.passOp);
	info.StencilFunc = getCompareOp(desc.compareOp);
	// ignore others??
}

CUTE_NS_END
