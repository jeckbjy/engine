#pragma once
#include "VK_API.h"

CUTE_NS_BEGIN

class CUTE_VK_API  VK_Mapping
{
public:
	static VkFormat					getFormat(RESOURCE_FORMAT format);
	static VkBufferUsageFlags		getBufferUsage(RESOURCE_USAGE usage);
	static VkImageUsageFlags		getImageUsage(RESOURCE_USAGE usage);
	static VkImageType				getImageType(RESOURCE_DIMENSION dimension);
	static VkImageViewType			getImageViewType(VIEW_DIMENSION value);
	static VkImageAspectFlags		getImageAspect(TEXTURE_ASPECT aspect);
	static VkComponentSwizzle		getComponentSwizzle(TEXTURE_SWIZZLE value);
	static VkPrimitiveTopology		getTopology(PRIMITIVE_TOPOLOGY topology);
	static VkSampleCountFlagBits	getSampleCountFlagBits(uint32 sampleCount);
	static VkMemoryPropertyFlags	getMemoryPropertyFlags(CPU_PAGE_PROPERTY prop, bool isMappable);

	static VkPolygonMode			getPolygonMode(POLYGON_MODE mode);
	static VkCullModeFlags			getCullMode(CULL_MODE mode);
	//static VkFrontFace				getFrontFace(FrontFace face);
	static VkCompareOp				getCompareOp(COMPARE_OP op);
	static VkStencilOp				getStencilOp(STENCIL_OP op);
	static VkLogicOp				getLogicOp(LOGIC_OP op);
	static VkBlendOp				getBlendOp(BLEND_OP op);
	static VkBlendFactor			getBlendFactor(BLEND_FACTOR factor);
	static VkFilter					getFilter(FILTER_MODE filter);
	static VkSamplerMipmapMode		getMipmapMode(MIPMAP_MODE filter);
	static VkSamplerAddressMode		getAddressMode(TEXTURE_ADDRESS_MODE mode);
	static VkBorderColor			getBorderColor(BORDER_COLOR color);

	//static void fillSamplerDesc(VkSamplerCreateInfo& info, const SamplerDesc& desc);

	//static void fillInputAssemblyState(VkPipelineInputAssemblyStateCreateInfo& info, Topology topology, bool primitiveRestartEnable = false);
	//static void fillTessellationState(VkPipelineTessellationStateCreateInfo& info, Topology topology);
	//static void fillRasterizationState(VkPipelineRasterizationStateCreateInfo& rast_info, const RasterizerDesc& rast_desc);
	//static void fillMultisampleState(VkPipelineMultisampleStateCreateInfo& info, const MultisampleDesc& desc);
	//static void fillDepthStencilState(VkPipelineDepthStencilStateCreateInfo& info, const DepthStencilDesc& desc);
	//static void fillStencilOpState(VkStencilOpState& state, const StencilOpDesc& desc);
	//static void fillBlendState(VkPipelineColorBlendStateCreateInfo& info, VkPipelineColorBlendAttachmentState* attachments, const BlendDesc& desc);
	//static void fillBlendTarget(VkPipelineColorBlendAttachmentState& info, const BlendTargetDesc& desc);
	//static void fillViewportState(VkPipelineViewportStateCreateInfo& info, uint32_t viewportCount = 1, uint32_t scissorCount = 1);
	//static void fillDynamicState(VkPipelineDynamicStateCreateInfo& info, VkDynamicState* states, uint32_t count);
};

CUTE_NS_END
