#pragma once
#include "D3D12_API.h"

CUTE_NS_BEGIN

class CUTE_D3D12_API D3D12_Mapping
{
public:
	static DXGI_FORMAT						getFormat(RESOURCE_FORMAT format);
	static D3D12_RESOURCE_DIMENSION			getTextureType(RESOURCE_DIMENSION type);
	static D3D12_PRIMITIVE_TOPOLOGY			getTopology(PRIMITIVE_TOPOLOGY topology);
	static D3D12_PRIMITIVE_TOPOLOGY_TYPE	getTopologyType(PRIMITIVE_TOPOLOGY topology);
	static D3D_PRIMITIVE_TOPOLOGY			getPrimitive(PRIMITIVE_TOPOLOGY topology, uint32_t controlCount);

	static D3D12_FILL_MODE					getFillMode(POLYGON_MODE mode);
	static D3D12_CULL_MODE					getCullMode(CULL_MODE mode);

	static D3D12_BLEND						getBlendFactor(BLEND_FACTOR factor);
	static D3D12_BLEND_OP					getBlendOp(BLEND_OP op);
	static D3D12_LOGIC_OP					getLogicOp(LOGIC_OP op);
	static D3D12_STENCIL_OP					getStencilOp(STENCIL_OP op);
	static D3D12_COMPARISON_FUNC			getCompareOp(COMPARE_OP op);
	static D3D12_TEXTURE_ADDRESS_MODE		getAddressMode(TEXTURE_ADDRESS_MODE mode);

	static D3D12_INPUT_CLASSIFICATION		getInputClassification(INPUT_CLASSIFICATION classification);
	static D3D12_COMMAND_LIST_TYPE			getCommandListType(COMMANDLIST_TYPE type);
	static D3D12_HEAP_TYPE					getHeapType(HEAP_TYPE type);
	static D3D12_CPU_PAGE_PROPERTY			getCpuPageProperty(CPU_PAGE_PROPERTY prop);
	static D3D12_RESOURCE_FLAGS				getResourceFlags(RESOURCE_USAGE usage);
	static D3D12_RESOURCE_STATES			getResourceStates(RESOURCE_STATE state);
	static D3D12_RESOURCE_DIMENSION			getResorceDimension(RESOURCE_DIMENSION dimension);
	static D3D12_FILTER						getFilter(FILTER_MODE min, FILTER_MODE mag, MIPMAP_MODE mip, bool compare, bool anisotropy);
	static void								getBorderColor(BORDER_COLOR color, FLOAT* result);

	//static void fillShader(D3D12_SHADER_BYTECODE& code, ShaderStage* prog);
	//static void fillRasterizerState(D3D12_RASTERIZER_DESC& state, const RasterizerDesc& desc);
	//static void fillBlendState(D3D12_BLEND_DESC& state, const BlendDesc& desc);
	//static void fillBlendTarget(D3D12_RENDER_TARGET_BLEND_DESC& state, const BlendTargetDesc& desc);
	//static void fillDepthStencilState(D3D12_DEPTH_STENCIL_DESC& state, const DepthStencilDesc& desc);
	//static void fillStencilOpState(D3D12_DEPTH_STENCILOP_DESC& state, const StencilOpDesc& desc);
	//static void fillSamplerDesc(D3D12_SAMPLER_DESC& info, const SamplerDesc& desc);

	//static void fillSemantic(Semantic semantic, LPCSTR& name, UINT& index);
};

CUTE_NS_END
