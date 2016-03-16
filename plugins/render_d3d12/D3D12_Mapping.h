#pragma once
#include "D3D12_API.h"

CU_NS_BEGIN

class CU_D3D12_API D3D12Mapping
{
public:
	static DXGI_FORMAT getFormat(PixelFormat format);
	static D3D12_RESOURCE_DIMENSION getTextureType(TexType type);
	static D3D12_PRIMITIVE_TOPOLOGY getTopology(Topology topology);
	static D3D12_PRIMITIVE_TOPOLOGY_TYPE getTopologyType(Topology topology);

	static D3D12_FILL_MODE getFillMode(FillMode mode);
	static D3D12_CULL_MODE getCullMode(CullMode mode);

	static D3D12_LOGIC_OP	getLogicOp(LogicOp op);
	static D3D12_BLEND_OP	getBlendOp(BlendOp op);
	static D3D12_BLEND		getBlendFactor(BlendFactor factor);
	static D3D12_STENCIL_OP getStencilOp(StencilOp op);
	static D3D12_COMPARISON_FUNC getCompareOp(CompareOp op);

	static void fillShader(D3D12_SHADER_BYTECODE& code, Program* prog);
	static void fillRasterizerState(D3D12_RASTERIZER_DESC& state, const RasterizerDesc& desc);
	static void fillBlendState(D3D12_BLEND_DESC& state, const BlendDesc& desc);
	static void fillBlendTarget(D3D12_RENDER_TARGET_BLEND_DESC& state, const BlendTargetDesc& desc);
	static void fillDepthStencilState(D3D12_DEPTH_STENCIL_DESC& state, const DepthStencilDesc& desc);
	static void fillStencilOpState(D3D12_DEPTH_STENCILOP_DESC& state, const StencilOpState& desc);
};

CU_NS_END
