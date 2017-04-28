#include "VK_Mapping.h"

CUTE_NS_BEGIN

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

VkFilter VK_Mapping::getFilter(FilterType filter)
{
	return (VkFilter)filter;
}

VkSamplerMipmapMode VK_Mapping::getMipmapMode(FilterType filter)
{
	return (VkSamplerMipmapMode)filter;
}

VkSamplerAddressMode VK_Mapping::getAddressMode(AddressMode mode)
{
	return (VkSamplerAddressMode)mode;
}

void VK_Mapping::fillInputAssemblyState(VkPipelineInputAssemblyStateCreateInfo& info, Topology topology, bool primitiveRestartEnable /* = false */)
{
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	info.pNext = NULL;
	info.flags = 0;
	info.topology = getTopology(topology);
	info.primitiveRestartEnable = primitiveRestartEnable;
}

void VK_Mapping::fillTessellationState(VkPipelineTessellationStateCreateInfo& info, Topology topology)
{
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
	info.pNext = NULL;
	info.flags = 0;
	info.patchControlPoints = topology < PT_CTRL_PATCH_LIST1 ? 0 : (uint32_t)(topology - PT_CTRL_PATCH_LIST1);
}

void VK_Mapping::fillRasterizationState(VkPipelineRasterizationStateCreateInfo& info, const RasterizerDesc& desc)
{
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	info.pNext = NULL;
	info.flags = 0;
	info.polygonMode = getPolygonMode(desc.fillMode);
	info.cullMode = getCullMode(desc.cullMode);
	info.frontFace = getFrontFace(desc.frontFace);
	info.rasterizerDiscardEnable = desc.discardEnable;
	info.depthClampEnable = desc.depthClampEnable;
	info.depthBiasEnable = desc.depthBiasEnable;
	info.depthBiasConstantFactor = desc.depthBiasConstantFactor;
	info.depthBiasClamp = desc.depthBiasSlopeFactor;
	info.lineWidth = desc.lineWidth;
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

void VK_Mapping::fillStencilOpState(VkStencilOpState& state, const StencilOpDesc& desc)
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

void VK_Mapping::fillSamplerDesc(VkSamplerCreateInfo& info, const SamplerDesc& desc)
{
	info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	info.pNext = NULL;
	info.flags = 0;
	info.magFilter = getFilter(desc.magFilter);
	info.minFilter = getFilter(desc.minFilter);
	info.mipmapMode = getMipmapMode(desc.mipFilter);
	info.addressModeU = getAddressMode(desc.addrU);
	info.addressModeV = getAddressMode(desc.addrV);
	info.addressModeW = getAddressMode(desc.addrW);
	info.mipLodBias = desc.mipLodBias;
	info.anisotropyEnable = desc.anisotropyEnable;
	info.maxAnisotropy = desc.maxAnisotropy;
	info.compareEnable = (desc.compareOp == CMP_OP_NEVER);
	info.compareOp = getCompareOp(desc.compareOp);
	info.minLod = desc.minLod;
	info.maxLod = desc.maxLod;
	// todo:convert
	info.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
	info.unnormalizedCoordinates = desc.unnormalizedCoordinates;
}

void VK_Mapping::fillViewportState(VkPipelineViewportStateCreateInfo& info, uint32_t viewportCount, uint32_t scissorCount)
{
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	info.pNext = NULL;
	info.flags = 0;
	info.viewportCount = viewportCount;
	info.pViewports = NULL;
	info.scissorCount = scissorCount;
	info.pScissors = NULL;
}

void VK_Mapping::fillDynamicState(VkPipelineDynamicStateCreateInfo& info, VkDynamicState* states, uint32_t count)
{
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	info.pNext = NULL;
	info.flags = 0;
	info.dynamicStateCount = count;
	info.pDynamicStates = states;
}

CUTE_NS_END
