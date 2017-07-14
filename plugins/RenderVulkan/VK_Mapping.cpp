#include "VK_Mapping.h"

CUTE_NS_BEGIN

VkFormat VK_Mapping::getFormat(RESOURCE_FORMAT format)
{
	static const VkFormat table[] = 
	{
		VK_FORMAT_UNDEFINED,
		VK_FORMAT_R32G32B32A32_SFLOAT,
		VK_FORMAT_R32G32B32A32_UINT,
		VK_FORMAT_R32G32B32A32_SINT,
		VK_FORMAT_R16G16B16A16_SFLOAT,
		VK_FORMAT_R16G16B16A16_UINT,
		VK_FORMAT_R16G16B16A16_SINT,
		VK_FORMAT_R32G32B32_SFLOAT,
		VK_FORMAT_R32G32B32_UINT,
		VK_FORMAT_R32G32B32_SINT,
		VK_FORMAT_R32G32_SFLOAT,
		VK_FORMAT_R32G32_UINT,
		VK_FORMAT_R32G32_SINT,
		VK_FORMAT_R16G16_SFLOAT,
		VK_FORMAT_R16G16_UINT,
		VK_FORMAT_R16G16_SINT,
		VK_FORMAT_R32_SFLOAT,
		VK_FORMAT_R32_UINT,
		VK_FORMAT_R32_SINT,
		VK_FORMAT_R16_SFLOAT,
		VK_FORMAT_R16_UINT,
		VK_FORMAT_R16_SINT,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_FORMAT_B8G8R8A8_SRGB,
		VK_FORMAT_B8G8R8A8_UNORM,
		VK_FORMAT_R8G8_UNORM,
		VK_FORMAT_R8_UNORM,
		VK_FORMAT_D32_SFLOAT,
		VK_FORMAT_D24_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM,
		VK_FORMAT_BC1_RGBA_SRGB_BLOCK,
		VK_FORMAT_BC1_RGBA_UNORM_BLOCK,
		VK_FORMAT_BC2_SRGB_BLOCK,
		VK_FORMAT_BC2_UNORM_BLOCK,
		VK_FORMAT_BC3_SRGB_BLOCK,
		VK_FORMAT_BC3_UNORM_BLOCK,
		VK_FORMAT_BC4_UNORM_BLOCK,
		VK_FORMAT_BC4_SNORM_BLOCK,
		VK_FORMAT_BC5_UNORM_BLOCK,
		VK_FORMAT_BC5_SNORM_BLOCK,
		VK_FORMAT_BC6H_UFLOAT_BLOCK,
		VK_FORMAT_BC6H_SFLOAT_BLOCK,
		VK_FORMAT_BC7_SRGB_BLOCK,
		VK_FORMAT_BC7_UNORM_BLOCK,
		VK_FORMAT_ASTC_4x4_SRGB_BLOCK,
		VK_FORMAT_ASTC_4x4_UNORM_BLOCK,
		VK_FORMAT_ASTC_5x4_SRGB_BLOCK,
		VK_FORMAT_ASTC_5x4_UNORM_BLOCK,
		VK_FORMAT_ASTC_5x5_SRGB_BLOCK,
		VK_FORMAT_ASTC_5x5_UNORM_BLOCK,
		VK_FORMAT_ASTC_6x5_SRGB_BLOCK,
		VK_FORMAT_ASTC_6x5_UNORM_BLOCK,
		VK_FORMAT_ASTC_6x6_SRGB_BLOCK,
		VK_FORMAT_ASTC_6x6_UNORM_BLOCK,
		VK_FORMAT_ASTC_8x5_SRGB_BLOCK,
		VK_FORMAT_ASTC_8x5_UNORM_BLOCK,
		VK_FORMAT_ASTC_8x6_SRGB_BLOCK,
		VK_FORMAT_ASTC_8x6_UNORM_BLOCK,
		VK_FORMAT_ASTC_8x8_SRGB_BLOCK,
		VK_FORMAT_ASTC_8x8_UNORM_BLOCK,
		VK_FORMAT_ASTC_10x5_SRGB_BLOCK,
		VK_FORMAT_ASTC_10x5_UNORM_BLOCK,
		VK_FORMAT_ASTC_10x6_SRGB_BLOCK,
		VK_FORMAT_ASTC_10x6_UNORM_BLOCK,
		VK_FORMAT_ASTC_10x8_SRGB_BLOCK,
		VK_FORMAT_ASTC_10x8_UNORM_BLOCK,
		VK_FORMAT_ASTC_10x10_SRGB_BLOCK,
		VK_FORMAT_ASTC_10x10_UNORM_BLOCK,
		VK_FORMAT_ASTC_12x10_SRGB_BLOCK,
		VK_FORMAT_ASTC_12x10_UNORM_BLOCK,
		VK_FORMAT_ASTC_12x12_SRGB_BLOCK,
		VK_FORMAT_ASTC_12x12_UNORM_BLOCK,
		VK_FORMAT_A2B10G10R10_UNORM_PACK32,
		VK_FORMAT_A2B10G10R10_UINT_PACK32,
		VK_FORMAT_B10G11R11_UFLOAT_PACK32,
	};

	return table[format];
}

VkBufferUsageFlags VK_Mapping::getBufferUsage(RESOURCE_USAGE usage)
{
	VkBufferUsageFlags result = 0;

	if (usage & RESOURCE_USAGE_UNORDERD_ACCESS)
		result |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;

	if (usage & RESOURCE_USAGE_INDEX_BUFFER)
		result |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

	if (usage & RESOURCE_USAGE_VERTEX_BUFFER)
		result |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

	if (usage & RESOURCE_USAGE_CONSTANT_BUFFER)
		result |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

	if (usage & RESOURCE_USAGE_INDIRECT_BUFFER)
		result |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;

	if (usage & RESOURCE_USAGE_COPY_SRC)
		result |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

	if (usage & RESOURCE_USAGE_COPY_DST)
		result |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;

	if (usage & RESOURCE_USAGE_QUERY_BUFFER)
		result |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;

	return result;
}

VkImageUsageFlags VK_Mapping::getImageUsage(RESOURCE_USAGE usage)
{
	VkImageUsageFlags result = 0;

	if (usage & RESOURCE_USAGE_COLOR_TARGET)
		result |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	if (usage & RESOURCE_USAGE_DEPTH_TARGET)
		result |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

	if (usage & RESOURCE_USAGE_SHADER_RESOURCE)
		result |= VK_IMAGE_USAGE_SAMPLED_BIT;

	if (usage & RESOURCE_USAGE_COPY_SRC)
		result |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

	if (usage & RESOURCE_USAGE_COPY_DST)
		result |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;

	if (usage & RESOURCE_USAGE_UNORDERD_ACCESS)
	{
		// TODO : Òª´_ÕJ.
		result |= VK_IMAGE_USAGE_STORAGE_BIT;
		result |= VK_IMAGE_USAGE_SAMPLED_BIT;
	}

	return result;
}

VkImageType VK_Mapping::getImageType(RESOURCE_DIMENSION dimension)
{
	VkImageType table[] = 
	{
		VK_IMAGE_TYPE_1D,       // UNKNOWN
		VK_IMAGE_TYPE_1D,       // BUFFER
		VK_IMAGE_TYPE_1D,       // TEXTURE1D
		VK_IMAGE_TYPE_2D,       // TEXTURE2D
		VK_IMAGE_TYPE_3D,       // TEXTURE3D
		VK_IMAGE_TYPE_2D,       // CUBEMAP
	};

	return table[dimension];
}

VkImageViewType VK_Mapping::getImageViewType(VIEW_DIMENSION value)
{
	static VkImageViewType table[] = 
	{
		VK_IMAGE_VIEW_TYPE_1D,              // VIEW_DIMENSION_BUFFER
		VK_IMAGE_VIEW_TYPE_1D,              // VIEW_DIMENSION_TEXTURE1D
		VK_IMAGE_VIEW_TYPE_1D_ARRAY,        // VIEW_DIMENSION_TEXTURE1D_ARRAY
		VK_IMAGE_VIEW_TYPE_2D,              // VIEW_DIMENSION_TEXTURE2D
		VK_IMAGE_VIEW_TYPE_2D_ARRAY,        // VIEW_DIMENSION_TEXTURE2D_ARRAY
		VK_IMAGE_VIEW_TYPE_CUBE,            // VIEW_DIMENSION_CUBEMAP
		VK_IMAGE_VIEW_TYPE_CUBE_ARRAY,      // VIEW_DIMENSION_CUBEMAP_ARRAY
		VK_IMAGE_VIEW_TYPE_3D               // VIEW_DIMENSION_TEXTURE3D
	};

	return table[value];
}

VkImageAspectFlags VK_Mapping::getImageAspect(TEXTURE_ASPECT aspect)
{
	VkImageAspectFlags result = 0;
	switch (aspect)
	{
	case TEXTURE_ASPECT_COLOR:
		result = VK_IMAGE_ASPECT_COLOR_BIT;
		break;
	case TEXTURE_ASPECT_DEPTH:
		result = VK_IMAGE_ASPECT_DEPTH_BIT;
		break;
	case TEXTURE_ASPECT_STENCIL:
		result = VK_IMAGE_ASPECT_STENCIL_BIT;
		break;
	}

	return result;
}

VkComponentSwizzle VK_Mapping::getComponentSwizzle(TEXTURE_SWIZZLE value)
{
	static VkComponentSwizzle table[] = 
	{
		VK_COMPONENT_SWIZZLE_R,     // SWIZZLE_R
		VK_COMPONENT_SWIZZLE_G,     // SWIZZLE_G
		VK_COMPONENT_SWIZZLE_B,     // SWIZZLE_B
		VK_COMPONENT_SWIZZLE_A,     // SWIZZLE_A
		VK_COMPONENT_SWIZZLE_ZERO,  // SWIZZLE_ZERO
		VK_COMPONENT_SWIZZLE_ONE    // SWIZZLE_ONE
	};

	return table[value];
}

VkPrimitiveTopology VK_Mapping::getTopology(PRIMITIVE_TOPOLOGY topology)
{
	VkPrimitiveTopology table[] = 
	{
		VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
		VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
		VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
		VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
		VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY,
		VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY,
		VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY,
		VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY,
		VK_PRIMITIVE_TOPOLOGY_PATCH_LIST
	};

	return table[topology];
}

VkSampleCountFlagBits VK_Mapping::getSampleCountFlagBits(uint32 sampleCount)
{
	if (sampleCount <= 1)
		return VK_SAMPLE_COUNT_1_BIT;
	else if (sampleCount <= 2)
		return VK_SAMPLE_COUNT_2_BIT;
	else if (sampleCount <= 4)
		return VK_SAMPLE_COUNT_4_BIT;
	else if (sampleCount <= 8)
		return VK_SAMPLE_COUNT_8_BIT;
	else if (sampleCount <= 16)
		return VK_SAMPLE_COUNT_16_BIT;
	else if (sampleCount <= 32)
		return VK_SAMPLE_COUNT_32_BIT;
	else
		return VK_SAMPLE_COUNT_64_BIT;
}

VkMemoryPropertyFlags VK_Mapping::getMemoryPropertyFlags(CPU_PAGE_PROPERTY prop, bool isMappable)
{
	VkMemoryPropertyFlags result = 0;

	if (prop == CPU_PAGE_PROPERTY_DEFAULT)
	{
		result |= isMappable ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT : VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	}
	else if (prop == CPU_PAGE_PROPERTY_NOT_AVAILABLE)
	{
		result |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	}
	else if (prop == CPU_PAGE_PROPERTY_WRITE_COMBINE)
	{
		result |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		result |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
		result |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	}
	else if (prop == CPU_PAGE_PROPERTY_WRITE_BACK)
	{
		result |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		result |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
		result |= VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
	}

	return result;
}

VkPolygonMode VK_Mapping::getPolygonMode(POLYGON_MODE mode)
{
	VkPolygonMode table[] = 
	{
		VK_POLYGON_MODE_FILL,
		VK_POLYGON_MODE_LINE
	};

	return table[mode];
}

VkCullModeFlags VK_Mapping::getCullMode(CULL_MODE mode)
{
	VkCullModeFlags table[] = 
	{
		VK_CULL_MODE_NONE,
		VK_CULL_MODE_FRONT_BIT,
		VK_CULL_MODE_BACK_BIT
	};

	return table[mode];
}

VkCompareOp VK_Mapping::getCompareOp(COMPARE_OP value)
{
	static VkCompareOp table[] = 
	{
		VK_COMPARE_OP_NEVER,
		VK_COMPARE_OP_LESS,
		VK_COMPARE_OP_EQUAL,
		VK_COMPARE_OP_LESS_OR_EQUAL,
		VK_COMPARE_OP_GREATER,
		VK_COMPARE_OP_GREATER_OR_EQUAL,
		VK_COMPARE_OP_ALWAYS
	};

	return table[value];
}

VkStencilOp VK_Mapping::getStencilOp(STENCIL_OP value)
{
	VkStencilOp table[] = 
	{
		VK_STENCIL_OP_KEEP,
		VK_STENCIL_OP_ZERO,
		VK_STENCIL_OP_REPLACE,
		VK_STENCIL_OP_INCREMENT_AND_CLAMP,
		VK_STENCIL_OP_DECREMENT_AND_CLAMP,
		VK_STENCIL_OP_INVERT,
		VK_STENCIL_OP_INCREMENT_AND_WRAP,
		VK_STENCIL_OP_DECREMENT_AND_WRAP,
	};

	return table[value];
}

VkLogicOp VK_Mapping::getLogicOp(LOGIC_OP value)
{
	VkLogicOp table[] = 
	{
		VK_LOGIC_OP_CLEAR,
		VK_LOGIC_OP_SET,
		VK_LOGIC_OP_COPY,
		VK_LOGIC_OP_COPY_INVERTED,
		VK_LOGIC_OP_NO_OP,
		VK_LOGIC_OP_INVERT,
		VK_LOGIC_OP_AND,
		VK_LOGIC_OP_NAND,
		VK_LOGIC_OP_OR,
		VK_LOGIC_OP_NOR,
		VK_LOGIC_OP_XOR,
		VK_LOGIC_OP_EQUIVALENT,
		VK_LOGIC_OP_AND_REVERSE,
		VK_LOGIC_OP_AND_INVERTED,
		VK_LOGIC_OP_OR_REVERSE,
		VK_LOGIC_OP_OR_INVERTED
	};

	return table[value];
}

VkBlendOp VK_Mapping::getBlendOp(BLEND_OP value)
{
	VkBlendOp table[] = 
	{
		VK_BLEND_OP_ADD,
		VK_BLEND_OP_SUBTRACT,
		VK_BLEND_OP_REVERSE_SUBTRACT,
		VK_BLEND_OP_MIN,
		VK_BLEND_OP_MAX
	};

	return table[value];
}

VkBlendFactor VK_Mapping::getBlendFactor(BLEND_FACTOR value)
{
	VkBlendFactor table[] = 
	{
		VK_BLEND_FACTOR_ZERO,
		VK_BLEND_FACTOR_ONE,
		VK_BLEND_FACTOR_SRC_COLOR,
		VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
		VK_BLEND_FACTOR_SRC_ALPHA,
		VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
		VK_BLEND_FACTOR_DST_ALPHA,
		VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
		VK_BLEND_FACTOR_DST_COLOR,
		VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
		VK_BLEND_FACTOR_SRC_ALPHA_SATURATE,
		VK_BLEND_FACTOR_SRC1_COLOR,
		VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR,
		VK_BLEND_FACTOR_SRC1_ALPHA,
		VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA,
	};

	return table[value];
}

VkFilter VK_Mapping::getFilter(FILTER_MODE mode)
{
	VkFilter table[] = 
	{
		VK_FILTER_NEAREST,
		VK_FILTER_LINEAR
	};

	return table[mode];
}

VkSamplerMipmapMode VK_Mapping::getMipmapMode(MIPMAP_MODE mode)
{
	VkSamplerMipmapMode table[] = 
	{
		VK_SAMPLER_MIPMAP_MODE_NEAREST,
		VK_SAMPLER_MIPMAP_MODE_LINEAR
	};

	return table[mode];
}

VkSamplerAddressMode VK_Mapping::getAddressMode(TEXTURE_ADDRESS_MODE mode)
{
	VkSamplerAddressMode table[] = 
	{
		VK_SAMPLER_ADDRESS_MODE_REPEAT,
		VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
		VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
		VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
		VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE
	};

	return table[mode];
}

VkBorderColor VK_Mapping::getBorderColor(BORDER_COLOR color)
{
	VkBorderColor table[] = 
	{
		VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK,
		VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK,
		VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE
	};

	return table[color];
}

//void VK_Mapping::fillInputAssemblyState(VkPipelineInputAssemblyStateCreateInfo& info, Topology topology, bool primitiveRestartEnable /* = false */)
//{
//	info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
//	info.pNext = NULL;
//	info.flags = 0;
//	info.topology = getTopology(topology);
//	info.primitiveRestartEnable = primitiveRestartEnable;
//}
//
//void VK_Mapping::fillTessellationState(VkPipelineTessellationStateCreateInfo& info, Topology topology)
//{
//	info.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
//	info.pNext = NULL;
//	info.flags = 0;
//	info.patchControlPoints = topology < PT_CTRL_PATCH_LIST1 ? 0 : (uint32_t)(topology - PT_CTRL_PATCH_LIST1);
//}
//
//void VK_Mapping::fillRasterizationState(VkPipelineRasterizationStateCreateInfo& info, const RasterizerDesc& desc)
//{
//	info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
//	info.pNext = NULL;
//	info.flags = 0;
//	info.polygonMode = getPolygonMode(desc.fillMode);
//	info.cullMode = getCullMode(desc.cullMode);
//	info.frontFace = getFrontFace(desc.frontFace);
//	info.rasterizerDiscardEnable = desc.discardEnable;
//	info.depthClampEnable = desc.depthClampEnable;
//	info.depthBiasEnable = desc.depthBiasEnable;
//	info.depthBiasConstantFactor = desc.depthBiasConstantFactor;
//	info.depthBiasClamp = desc.depthBiasSlopeFactor;
//	info.lineWidth = desc.lineWidth;
//}
//
//void VK_Mapping::fillMultisampleState(VkPipelineMultisampleStateCreateInfo& info, const MultisampleDesc& desc)
//{
//	info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
//	info.pNext = NULL;
//
//}
//
//void VK_Mapping::fillDepthStencilState(VkPipelineDepthStencilStateCreateInfo& info, const DepthStencilDesc& desc)
//{
//	info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
//	info.pNext = NULL;
//
//	info.depthTestEnable = desc.depthTestEnable;
//	info.depthWriteEnable = desc.depthWriteEnable;
//	info.depthBoundsTestEnable = desc.depthBoundsTestEnable;
//	info.depthCompareOp = getCompareOp(desc.depthCompareOp);
//	info.minDepthBounds = desc.minDepthBounds;
//	info.maxDepthBounds = desc.maxDepthBounds;
//	fillStencilOpState(info.front, desc.front);
//	fillStencilOpState(info.back, desc.back);
//}
//
//void VK_Mapping::fillStencilOpState(VkStencilOpState& state, const StencilOpDesc& desc)
//{
//	state.failOp = getStencilOp(desc.failOp);
//	state.passOp = getStencilOp(desc.passOp);
//	state.depthFailOp = getStencilOp(desc.depthFailOp);
//	state.compareOp = getCompareOp(desc.compareOp);
//	state.compareMask = desc.compareMask;
//	state.writeMask = desc.writeMask;
//	state.reference = desc.reference;
//}
//
//void VK_Mapping::fillBlendState(VkPipelineColorBlendStateCreateInfo& info, VkPipelineColorBlendAttachmentState* attachments, const BlendDesc& desc)
//{
//	info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
//	info.pNext = NULL;
//	info.flags = 0;
//	info.logicOpEnable = desc.logicOpEnable;
//	info.logicOp = getLogicOp(desc.logicOp);
//	memcpy(info.blendConstants, desc.blendConstants, sizeof(desc.blendConstants));
//	
//	uint32_t count = 8;
//	for (uint32_t i = 0; i < 8; ++i)
//	{
//		const BlendTargetDesc& target = desc.targets[i];
//		if (!target.blendEnable)
//		{
//			count = i;
//			break;
//		}
//		fillBlendTarget(attachments[i], target);
//	}
//	info.attachmentCount = count;
//	info.pAttachments = attachments;
//}
//
//void VK_Mapping::fillBlendTarget(VkPipelineColorBlendAttachmentState& info, const BlendTargetDesc& desc)
//{
//	info.blendEnable = desc.blendEnable;
//	
//	info.srcColorBlendFactor = getBlendFactor(desc.srcColorFactor);
//	info.dstColorBlendFactor = getBlendFactor(desc.dstColorFactor);
//	info.colorBlendOp = getBlendOp(desc.colorOp);
//
//	info.srcAlphaBlendFactor = getBlendFactor(desc.srcAlphaFactor);
//	info.dstAlphaBlendFactor = getBlendFactor(desc.dstAlphaFactor);
//	info.alphaBlendOp = getBlendOp(desc.alphaOp);
//	info.colorWriteMask = (VkColorComponentFlags)desc.colorWriteMask;
//}
//
//void VK_Mapping::fillSamplerDesc(VkSamplerCreateInfo& info, const SamplerDesc& desc)
//{
//	info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
//	info.pNext = NULL;
//	info.flags = 0;
//	info.magFilter = getFilter(desc.magFilter);
//	info.minFilter = getFilter(desc.minFilter);
//	info.mipmapMode = getMipmapMode(desc.mipFilter);
//	info.addressModeU = getAddressMode(desc.addrU);
//	info.addressModeV = getAddressMode(desc.addrV);
//	info.addressModeW = getAddressMode(desc.addrW);
//	info.mipLodBias = desc.mipLodBias;
//	info.anisotropyEnable = desc.anisotropyEnable;
//	info.maxAnisotropy = desc.maxAnisotropy;
//	info.compareEnable = (desc.compareOp == CMP_OP_NEVER);
//	info.compareOp = getCompareOp(desc.compareOp);
//	info.minLod = desc.minLod;
//	info.maxLod = desc.maxLod;
//	// todo:convert
//	info.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
//	info.unnormalizedCoordinates = desc.unnormalizedCoordinates;
//}
//
//void VK_Mapping::fillViewportState(VkPipelineViewportStateCreateInfo& info, uint32_t viewportCount, uint32_t scissorCount)
//{
//	info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
//	info.pNext = NULL;
//	info.flags = 0;
//	info.viewportCount = viewportCount;
//	info.pViewports = NULL;
//	info.scissorCount = scissorCount;
//	info.pScissors = NULL;
//}
//
//void VK_Mapping::fillDynamicState(VkPipelineDynamicStateCreateInfo& info, VkDynamicState* states, uint32_t count)
//{
//	info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
//	info.pNext = NULL;
//	info.flags = 0;
//	info.dynamicStateCount = count;
//	info.pDynamicStates = states;
//}

CUTE_NS_END
