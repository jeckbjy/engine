#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class CU_VK_API  VK_Mapping
{
public:
	static VkFormat getFormat(PixelFormat format);
	static VkBufferUsageFlags getBufferUsage(BufferUsage usage);
	static VkImageType getImageType(TexType type);
	static VkImageUsageFlagBits getImageUsage(TextureUsage usage);
	static VkPrimitiveTopology	getTopology(Topology topology);

	static VkPolygonMode	getPolygonMode(FillMode mode);
	static VkCullModeFlags	getCullMode(CullMode mode);
	static VkFrontFace		getFrontFace(FrontFace face);
	static VkCompareOp		getCompareOp(CompareOp op);
	static VkStencilOp		getStencilOp(StencilOp op);
	static VkLogicOp		getLogicOp(LogicOp op);
	static VkBlendOp		getBlendOp(BlendOp op);
	static VkBlendFactor	getBlendFactor(BlendFactor factor);
	static VkFilter			getFilter(FilterType filter);
	static VkSamplerMipmapMode getMipmapMode(FilterType filter);
	static VkSamplerAddressMode getAddressMode(AddressMode mode);

	static void fillSamplerDesc(VkSamplerCreateInfo& info, const SamplerDesc& desc);

	static void fillInputAssemblyState(VkPipelineInputAssemblyStateCreateInfo& info, Topology topology, bool primitiveRestartEnable = false);
	static void fillTessellationState(VkPipelineTessellationStateCreateInfo& info, Topology topology);
	static void fillRasterizationState(VkPipelineRasterizationStateCreateInfo& rast_info, const RasterizerDesc& rast_desc);
	static void fillMultisampleState(VkPipelineMultisampleStateCreateInfo& info, const MultisampleDesc& desc);
	static void fillDepthStencilState(VkPipelineDepthStencilStateCreateInfo& info, const DepthStencilDesc& desc);
	static void fillStencilOpState(VkStencilOpState& state, const StencilOpDesc& desc);
	static void fillBlendState(VkPipelineColorBlendStateCreateInfo& info, VkPipelineColorBlendAttachmentState* attachments, const BlendDesc& desc);
	static void fillBlendTarget(VkPipelineColorBlendAttachmentState& info, const BlendTargetDesc& desc);
	static void fillViewportState(VkPipelineViewportStateCreateInfo& info, uint32_t viewportCount = 1, uint32_t scissorCount = 1);
	static void fillDynamicState(VkPipelineDynamicStateCreateInfo& info, VkDynamicState* states, uint32_t count);
};

CU_NS_END
