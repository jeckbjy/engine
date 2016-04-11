#include "D3D11_Mapping.h"

CU_NS_BEGIN

DXGI_FORMAT D3D11Mapping::getFormat(PixelFormat format)
{
	return DXGI_FORMAT_UNKNOWN;
}

D3D11_PRIMITIVE_TOPOLOGY D3D11Mapping::getTopoloty(Topology topology)
{
	return D3D11_PRIMITIVE_TOPOLOGY(topology);
}

D3D11_FILL_MODE D3D11Mapping::getFillMode(FillMode mode)
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

D3D11_CULL_MODE D3D11Mapping::getCullMode(CullMode mode)
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

D3D11_BLEND D3D11Mapping::getBlendFactor(BlendFactor factor)
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

D3D11_BLEND_OP D3D11Mapping::getBlendOp(BlendOp op)
{
	return (D3D11_BLEND_OP)op;
}

D3D11_LOGIC_OP D3D11Mapping::getLogicOp(LogicOp op)
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

D3D11_STENCIL_OP D3D11Mapping::getStencilOp(StencilOp op)
{
	return (D3D11_STENCIL_OP)op;
}

D3D11_COMPARISON_FUNC D3D11Mapping::getCompareOp(CompareOp op)
{
	return (D3D11_COMPARISON_FUNC)op;
}

D3D11_TEXTURE_ADDRESS_MODE D3D11Mapping::getAddressMode(AddressMode mode)
{
	return (D3D11_TEXTURE_ADDRESS_MODE)mode;
}

void D3D11Mapping::fillSemantic(Semantic semantic, LPCSTR& name, UINT& index)
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

void D3D11Mapping::fillSampler(D3D11_SAMPLER_DESC& info, const SamplerDesc& desc)
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

void D3D11Mapping::fillRasterizer(D3D11_RASTERIZER_DESC& info, const RasterizerDesc& desc)
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

void D3D11Mapping::fillBlend(D3D11_BLEND_DESC& info, const BlendDesc& desc)
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

void D3D11Mapping::fillBlendTarget(D3D11_RENDER_TARGET_BLEND_DESC& info, const BlendTargetDesc& desc)
{
	info.SrcBlend = getBlendFactor(desc.srcColorFactor);
	info.DestBlend = getBlendFactor(desc.dstColorFactor);
	info.BlendOp = getBlendOp(desc.colorOp);
	info.SrcBlendAlpha = getBlendFactor(desc.srcAlphaFactor);
	info.DestBlendAlpha = getBlendFactor(desc.dstAlphaFactor);
	info.BlendOpAlpha = getBlendOp(desc.alphaOp);
	info.RenderTargetWriteMask = (UINT8)desc.colorWriteMask;
}

void D3D11Mapping::fillDepthStencil(D3D11_DEPTH_STENCIL_DESC& info, const DepthStencilDesc& desc)
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

void D3D11Mapping::fillDepthStencilOp(D3D11_DEPTH_STENCILOP_DESC& info, const StencilOpDesc& desc)
{
	info.StencilFailOp = getStencilOp(desc.failOp);
	info.StencilDepthFailOp = getStencilOp(desc.depthFailOp);
	info.StencilPassOp = getStencilOp(desc.passOp);
	info.StencilFunc = getCompareOp(desc.compareOp);
	// ignore others??
}

CU_NS_END