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
	static VkPrimitiveTopology getTopology(Topology topology);
	static VkPolygonMode getPolygonMode(FillMode mode);
	static VkCullModeFlags getCullMode(CullMode mode);
	static VkFrontFace getFrontFace(FrontFace face);
	static VkCompareOp getCompareOp(CompareOp op);
	static VkStencilOp getStencilOp(StencilOp op);
	static VkLogicOp getLogicOp(LogicOp op);
	static VkBlendOp getBlendOp(BlendOp op);
	static VkBlendFactor getBlendFactor(BlendFactor factor);

	static void fillRasterizationState(VkPipelineRasterizationStateCreateInfo& rast_info, const RasterizerDesc& rast_desc);
	static void fillMultisampleState(VkPipelineMultisampleStateCreateInfo& info, const MultisampleDesc& desc);
	static void fillDepthStencilState(VkPipelineDepthStencilStateCreateInfo& info, const DepthStencilDesc& desc);
	static void fillStencilOpState(VkStencilOpState& state, const StencilOpState& desc);
	static void fillBlendState(VkPipelineColorBlendStateCreateInfo& info, VkPipelineColorBlendAttachmentState* attachments, const BlendDesc& desc);
	static void fillBlendTarget(VkPipelineColorBlendAttachmentState& info, const BlendTargetDesc& desc);
};

CU_NS_END
