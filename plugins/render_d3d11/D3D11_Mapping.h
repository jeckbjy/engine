#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class CU_D3D11_API D3D11_Mapping
{
public:
	static DXGI_FORMAT					getFormat(PixelFormat format);
	static D3D11_PRIMITIVE_TOPOLOGY		getTopoloty(Topology topology);

	static D3D11_FILL_MODE				getFillMode(FillMode mode);
	static D3D11_CULL_MODE				getCullMode(CullMode mode);

	static D3D11_BLEND					getBlendFactor(BlendFactor factor);
	static D3D11_BLEND_OP				getBlendOp(BlendOp op);
	static D3D11_LOGIC_OP				getLogicOp(LogicOp op);
	static D3D11_STENCIL_OP				getStencilOp(StencilOp op);
	static D3D11_COMPARISON_FUNC		getCompareOp(CompareOp op);
	static D3D11_TEXTURE_ADDRESS_MODE	getAddressMode(AddressMode mode);

	static DXGI_MODE_SCANLINE_ORDER		getScanlineOrder(ScanlineOrderMode mode);
	static DXGI_MODE_SCALING			getScaling(ScalingMode mode);
	static DXGI_SWAP_EFFECT				getSwapEffect(SwapMode mode);

	static void fillSemantic(Semantic semantic, LPCSTR& name, UINT& index);
	static void fillSampler(D3D11_SAMPLER_DESC& info, const SamplerDesc& desc);
	static void fillRasterizer(D3D11_RASTERIZER_DESC& info, const RasterizerDesc& desc);
	
	static void fillBlend(D3D11_BLEND_DESC& info, const BlendDesc& desc);
	static void fillBlendTarget(D3D11_RENDER_TARGET_BLEND_DESC& info, const BlendTargetDesc& desc);

	static void fillDepthStencil(D3D11_DEPTH_STENCIL_DESC& info, const DepthStencilDesc& desc);
	static void fillDepthStencilOp(D3D11_DEPTH_STENCILOP_DESC& info, const StencilOpDesc& desc);
};

CU_NS_END