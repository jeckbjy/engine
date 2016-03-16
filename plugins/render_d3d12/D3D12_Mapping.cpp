#include "D3D12_Mapping.h"

CU_NS_BEGIN

DXGI_FORMAT D3D12Mapping::getFormat(PixelFormat format)
{
	return DXGI_FORMAT_UNKNOWN;
}

D3D12_RESOURCE_DIMENSION D3D12Mapping::getTextureType(TexType type)
{
	switch (type)
	{
	case TEX_1D: return D3D12_RESOURCE_DIMENSION_TEXTURE1D;
	case TEX_2D: return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	case TEX_3D: return D3D12_RESOURCE_DIMENSION_TEXTURE3D;
	}

	return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
}

D3D12_PRIMITIVE_TOPOLOGY D3D12Mapping::getTopology(Topology topology)
{
	return D3D12_PRIMITIVE_TOPOLOGY(topology);
}

D3D12_PRIMITIVE_TOPOLOGY_TYPE D3D12Mapping::getTopologyType(Topology topology)
{
	// todo:convert?
	return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
}

D3D12_FILL_MODE D3D12Mapping::getFillMode(FillMode mode)
{
	switch (mode)
	{
	case FILL_MODE_SOLID:
		return D3D12_FILL_MODE_SOLID;
	case FILL_MODE_LINE:
		return D3D12_FILL_MODE_WIREFRAME;
	case FILL_MODE_POINT:// dx不支持点类型
		return D3D12_FILL_MODE_WIREFRAME;
	default:
		return D3D12_FILL_MODE_SOLID;
	}
}

D3D12_CULL_MODE D3D12Mapping::getCullMode(CullMode mode)
{
	switch (mode)
	{
	case CULL_MODE_FRONT:
		return D3D12_CULL_MODE_FRONT;
	case CULL_MODE_BACK:
		return D3D12_CULL_MODE_BACK;
	case CULL_MODE_BOTH:// dx不支持双面？
		return D3D12_CULL_MODE_BACK;
	default:
		return D3D12_CULL_MODE_FRONT;
	}
}

D3D12_LOGIC_OP D3D12Mapping::getLogicOp(LogicOp op)
{
	switch (op)
	{
	case LOGIC_OP_CLEAR:
		return D3D12_LOGIC_OP_CLEAR;
	case LOGIC_OP_AND:
		return D3D12_LOGIC_OP_AND;
	case LOGIC_OP_AND_REVERSE:
		return D3D12_LOGIC_OP_AND_REVERSE;
	case LOGIC_OP_COPY:
		return D3D12_LOGIC_OP_COPY;
	case LOGIC_OP_AND_INVERTED:
		return D3D12_LOGIC_OP_AND_INVERTED;
	case LOGIC_OP_NO_OP:
		return D3D12_LOGIC_OP_NOOP;
	case LOGIC_OP_XOR:
		return D3D12_LOGIC_OP_XOR;
	case LOGIC_OP_OR:
		return D3D12_LOGIC_OP_OR;
	case LOGIC_OP_NOR:
		return D3D12_LOGIC_OP_NOR;
	case LOGIC_OP_EQUIV:
		return D3D12_LOGIC_OP_EQUIV;
	case LOGIC_OP_INVERT:
		return D3D12_LOGIC_OP_INVERT;
	case LOGIC_OP_OR_REVERSE:
		return D3D12_LOGIC_OP_OR_REVERSE;
	case LOGIC_OP_COPY_INVERTED:
		return D3D12_LOGIC_OP_COPY_INVERTED;
	case LOGIC_OP_OR_INVERTED:
		return D3D12_LOGIC_OP_OR_INVERTED;
	case LOGIC_OP_NAND:
		return D3D12_LOGIC_OP_NAND;
	case LOGIC_OP_SET:
		return D3D12_LOGIC_OP_SET;
	default:
		return D3D12_LOGIC_OP_CLEAR;
	}
}

D3D12_BLEND_OP D3D12Mapping::getBlendOp(BlendOp op)
{
	return (D3D12_BLEND_OP)op;
}

D3D12_BLEND D3D12Mapping::getBlendFactor(BlendFactor factor)
{
	switch (factor)
	{
	case BLEND_FACTOR_ZERO:
		return D3D12_BLEND_ZERO;
	case BLEND_FACTOR_ONE:
		return D3D12_BLEND_ONE;
	case BLEND_FACTOR_SRC_COLOR:
		return D3D12_BLEND_SRC_COLOR;
	case BLEND_FACTOR_ONE_MINUS_SRC_COLOR:
		return D3D12_BLEND_INV_SRC_COLOR;
	case BLEND_FACTOR_SRC_ALPHA:
		return D3D12_BLEND_SRC_ALPHA;
	case BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:
		return D3D12_BLEND_INV_SRC_ALPHA;
	case BLEND_FACTOR_DST_COLOR:
		return D3D12_BLEND_DEST_COLOR;
	case BLEND_FACTOR_ONE_MINUS_DST_COLOR:
		return D3D12_BLEND_INV_DEST_COLOR;
	case BLEND_FACTOR_DST_ALPHA:
		return D3D12_BLEND_DEST_ALPHA;
	case BLEND_FACTOR_ONE_MINUS_DST_ALPHA:
		return D3D12_BLEND_INV_DEST_ALPHA;
	case BLEND_FACTOR_CONST_COLOR:
		return D3D12_BLEND_BLEND_FACTOR;
	case BLEND_FACTOR_ONE_MINUS_CONST_COLOR:
		return D3D12_BLEND_INV_BLEND_FACTOR;
	case BLEND_FACTOR_CONST_ALPHA:				// 不支持？？
		return D3D12_BLEND_BLEND_FACTOR;
	case BLEND_FACTOR_ONE_MINUS_CONST_ALPHA:	// 不支持？？
		return D3D12_BLEND_INV_BLEND_FACTOR;
	case BLEND_FACTOR_SRC_ALPHA_SATURATE:
		return D3D12_BLEND_SRC_ALPHA_SAT;
	case BLEND_FACTOR_SRC1_COLOR:
		return D3D12_BLEND_SRC1_COLOR;
	case BLEND_FACTOR_ONE_MINUS_SRC1_COLOR:
		return D3D12_BLEND_INV_SRC1_COLOR;
	case BLEND_FACTOR_SRC1_ALPHA:
		return D3D12_BLEND_SRC1_ALPHA;
	case BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA:
		return D3D12_BLEND_INV_SRC1_ALPHA;
	default:
		return D3D12_BLEND_ZERO;
	}
}

D3D12_STENCIL_OP D3D12Mapping::getStencilOp(StencilOp op)
{
	return (D3D12_STENCIL_OP)op;
}

D3D12_COMPARISON_FUNC D3D12Mapping::getCompareOp(CompareOp op)
{
	return (D3D12_COMPARISON_FUNC)op;
}

void D3D12Mapping::fillShader(D3D12_SHADER_BYTECODE& code, Program* prog)
{

}

void D3D12Mapping::fillRasterizerState(D3D12_RASTERIZER_DESC& state, const RasterizerDesc& desc)
{
	state.FillMode = getFillMode(desc.fillMode);
	state.CullMode = getCullMode(desc.cullMode);
	state.FrontCounterClockwise = (desc.frontFace == FRONT_FACE_COUNTER_CLOCKWISE);
	state.DepthBias = (INT)desc.depthBiasConstantFactor;
	state.DepthBiasClamp = desc.depthBiasClamp;
	state.DepthClipEnable = desc.depthClampEnable;
	// 如何对应??
	state.MultisampleEnable = FALSE;
	state.AntialiasedLineEnable = FALSE;
	state.ForcedSampleCount = 0;
	state.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
}

void D3D12Mapping::fillBlendState(D3D12_BLEND_DESC& state, const BlendDesc& desc)
{
	BOOL logicOpEnable = desc.logicOpEnable;
	D3D12_LOGIC_OP logicOp = getLogicOp(desc.logicOp);
	// target
	for (int i = 0; i < 8; ++i)
	{
		D3D12_RENDER_TARGET_BLEND_DESC& target = state.RenderTarget[i];
		target.LogicOpEnable = logicOpEnable;
		target.LogicOp = logicOp;
		fillBlendTarget(target, desc.targets[i]);
	}
}

void D3D12Mapping::fillBlendTarget(D3D12_RENDER_TARGET_BLEND_DESC& state, const BlendTargetDesc& desc)
{
	state.SrcBlend = getBlendFactor(desc.srcColorFactor);
	state.DestBlend = getBlendFactor(desc.dstColorFactor);
	state.BlendOp = getBlendOp(desc.colorOp);
	state.SrcBlendAlpha = getBlendFactor(desc.srcAlphaFactor);
	state.DestBlendAlpha = getBlendFactor(desc.dstAlphaFactor);
	state.BlendOpAlpha = getBlendOp(desc.alphaOp);
	state.RenderTargetWriteMask = (UINT8)desc.colorWriteMask;
}

void D3D12Mapping::fillDepthStencilState(D3D12_DEPTH_STENCIL_DESC& state, const DepthStencilDesc& desc)
{
	state.DepthEnable = desc.depthTestEnable;
	state.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	state.DepthFunc = getCompareOp(desc.depthCompareOp);
	state.StencilEnable = desc.stencilTestEnable;
	state.StencilReadMask = 0;
	state.StencilWriteMask = 0;
	fillStencilOpState(state.FrontFace, desc.front);
	fillStencilOpState(state.BackFace, desc.back);
}

void D3D12Mapping::fillStencilOpState(D3D12_DEPTH_STENCILOP_DESC& state, const StencilOpState& desc)
{
	state.StencilFailOp = getStencilOp(desc.failOp);
	state.StencilDepthFailOp = getStencilOp(desc.depthFailOp);
	state.StencilPassOp = getStencilOp(desc.passOp);
	state.StencilFunc = getCompareOp(desc.compareOp);
	// ignore others??
}

CU_NS_END
