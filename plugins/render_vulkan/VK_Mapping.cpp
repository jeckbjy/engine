#include "VK_Mapping.h"

CU_NS_BEGIN

VkFormat VK_Mapping::getFormat(PixelFormat format)
{
	return VK_FORMAT_UNDEFINED;
}

VkBufferUsageFlags VK_Mapping::getBufferUsage(BufferUsage usage)
{
	//switch (usage)
	//{
	//case BIND_VERTEX_BUFFER:
	//	return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	//case BIND_INDEX_BUFFER:
	//	return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	//case BIND_UNIFORM_BUFFER:
	//	return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	//}

	return 0;
}

VkImageType VK_Mapping::getImageType(TexType type)
{
	switch (type)
	{
	case TEX_1D: return VK_IMAGE_TYPE_1D;
	case TEX_2D: return VK_IMAGE_TYPE_2D;
	case TEX_3D: return VK_IMAGE_TYPE_3D;
	}
	return VK_IMAGE_TYPE_2D;
}

VkImageUsageFlagBits VK_Mapping::getImageUsage(TextureUsage usage)
{
	return VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
}

VkPrimitiveTopology VK_Mapping::getTopology(Topology topology)
{
	switch (topology)
	{
	case PT_POINT_LIST:
		return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
	case PT_LINE_LIST:
		return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
	case PT_LINE_STRIP:
		return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
	case PT_TRIANGLE_LIST:
		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	case PT_TRIANGLE_STRIP:
		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
	default:
		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	}
}

VkPolygonMode VK_Mapping::getPolygonMode(FillMode mode)
{
	return (VkPolygonMode)mode;
}

VkCullModeFlags VK_Mapping::getCullMode(CullMode mode)
{
	// “ª“ª”≥…‰
	return (VkCullModeFlags)mode;
}

VkFrontFace VK_Mapping::getFrontFace(FrontFace face)
{
	return (VkFrontFace)face;
}

VkCompareOp VK_Mapping::getCompareOp(CompareOp op)
{
	return (VkCompareOp)op;
}

VkStencilOp VK_Mapping::getStencilOp(StencilOp op)
{
	return (VkStencilOp)op;
}

VkLogicOp VK_Mapping::getLogicOp(LogicOp op)
{
	return (VkLogicOp)op;
}

VkBlendOp VK_Mapping::getBlendOp(BlendOp op)
{
	return (VkBlendOp)op;
}

VkBlendFactor VK_Mapping::getBlendFactor(BlendFactor factor)
{
	return (VkBlendFactor)factor;
}

void VK_Mapping::fillRasterizationState(VkPipelineRasterizationStateCreateInfo& rast_info, const RasterizerDesc& rast_desc)
{
	rast_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rast_info.pNext = NULL;
	rast_info.flags = 0;
	rast_info.polygonMode = getPolygonMode(rast_desc.fillMode);
	rast_info.cullMode = getCullMode(rast_desc.cullMode);
	rast_info.frontFace = getFrontFace(rast_desc.frontFace);
	rast_info.rasterizerDiscardEnable = rast_desc.discardEnable;
	rast_info.depthClampEnable = rast_desc.depthClampEnable;
	rast_info.depthBiasEnable = rast_desc.depthBiasEnable;
	rast_info.depthBiasConstantFactor = rast_desc.depthBiasConstantFactor;
	rast_info.depthBiasClamp = rast_desc.depthBiasSlopeFactor;
	rast_info.lineWidth = rast_desc.lineWidth;
}

void VK_Mapping::fillMultisampleState(VkPipelineMultisampleStateCreateInfo& info, const MultisampleDesc& desc)
{
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	info.pNext = NULL;

}

void VK_Mapping::fillDepthStencilState(VkPipelineDepthStencilStateCreateInfo& info, const DepthStencilDesc& desc)
{
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	info.pNext = NULL;

	info.depthTestEnable = desc.depthTestEnable;
	info.depthWriteEnable = desc.depthWriteEnable;
	info.depthBoundsTestEnable = desc.depthBoundsTestEnable;
	info.depthCompareOp = getCompareOp(desc.depthCompareOp);
	info.minDepthBounds = desc.minDepthBounds;
	info.maxDepthBounds = desc.maxDepthBounds;
	fillStencilOpState(info.front, desc.front);
	fillStencilOpState(info.back, desc.back);
}

void VK_Mapping::fillStencilOpState(VkStencilOpState& state, const StencilOpState& desc)
{
	state.failOp = getStencilOp(desc.failOp);
	state.passOp = getStencilOp(desc.passOp);
	state.depthFailOp = getStencilOp(desc.depthFailOp);
	state.compareOp = getCompareOp(desc.compareOp);
	state.compareMask = desc.compareMask;
	state.writeMask = desc.writeMask;
	state.reference = desc.reference;
}

void VK_Mapping::fillBlendState(VkPipelineColorBlendStateCreateInfo& info, VkPipelineColorBlendAttachmentState* attachments, const BlendDesc& desc)
{
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	info.pNext = NULL;
	info.flags = 0;
	info.logicOpEnable = desc.logicOpEnable;
	info.logicOp = getLogicOp(desc.logicOp);
	memcpy(info.blendConstants, desc.blendConstants, sizeof(desc.blendConstants));
	
	uint32_t count = 8;
	for (uint32_t i = 0; i < 8; ++i)
	{
		const BlendTargetDesc& target = desc.targets[i];
		if (!target.blendEnable)
		{
			count = i;
			break;
		}
		fillBlendTarget(attachments[i], target);
	}
	info.attachmentCount = count;
	info.pAttachments = attachments;
}

void VK_Mapping::fillBlendTarget(VkPipelineColorBlendAttachmentState& info, const BlendTargetDesc& desc)
{
	info.blendEnable = desc.blendEnable;
	
	info.srcColorBlendFactor = getBlendFactor(desc.srcColorFactor);
	info.dstColorBlendFactor = getBlendFactor(desc.dstColorFactor);
	info.colorBlendOp = getBlendOp(desc.colorOp);

	info.srcAlphaBlendFactor = getBlendFactor(desc.srcAlphaFactor);
	info.dstAlphaBlendFactor = getBlendFactor(desc.dstAlphaFactor);
	info.alphaBlendOp = getBlendOp(desc.alphaOp);
	info.colorWriteMask = (VkColorComponentFlags)desc.colorWriteMask;
}

CU_NS_END
