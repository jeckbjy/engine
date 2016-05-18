#include "GraphicsDesc.h"

CU_NS_BEGIN

TextureDesc::TextureDesc()
{

}

TextureDesc::TextureDesc(PixelFormat fmt, uint32_t width, uint32_t height, TextureUsage usage)
{

}

SamplerDesc::SamplerDesc()
	: magFilter(FILTER_TYPE_LINEAR)
	, minFilter(FILTER_TYPE_LINEAR)
	, mipFilter(FILTER_TYPE_LINEAR)
	, addrU(ADDRESS_MODE_CLAMP)
	, addrV(ADDRESS_MODE_CLAMP)
	, addrW(ADDRESS_MODE_CLAMP)
	, anisotropyEnable(false)
	, maxAnisotropy(1)
	, mipLodBias(0)
	, compareOp(CMP_OP_NEVER)
	, minLod(-3.402823466e+38F)
	, maxLod(3.402823466e+38F)
	, unnormalizedCoordinates(false)
{
	borderColor[0] = 1.0f;
	borderColor[1] = 1.0f;
	borderColor[2] = 1.0f;
	borderColor[3] = 1.0f;
}

bool SamplerDesc::operator ==(const SamplerDesc& other) const
{
	return
		this->magFilter == other.magFilter &&
		this->minFilter == other.minFilter &&
		this->mipFilter == other.mipFilter &&
		this->addrU == other.addrU &&
		this->addrV == other.addrV &&
		this->addrW == other.addrW &&
		this->anisotropyEnable == other.anisotropyEnable &&
		this->maxAnisotropy == other.maxAnisotropy &&
		this->mipLodBias == other.mipLodBias &&
		this->compareOp == other.compareOp &&
		this->minLod == other.minLod &&
		this->maxLod == other.maxLod &&
		this->unnormalizedCoordinates == other.unnormalizedCoordinates &&
		memcmp(this->borderColor, other.borderColor, sizeof(this->borderColor)) == 0;
}

RasterizerDesc::RasterizerDesc()
	: fillMode(FILL_MODE_SOLID)
	, cullMode(CULL_MODE_BACK)
	, frontFace(FRONT_FACE_COUNTER_CLOCKWISE)
	, discardEnable(false)
	, depthClampEnable(true)
	, depthBiasEnable(false)
	, depthBiasConstantFactor(0.0f)
	, depthBiasClamp(0.0f)
	, depthBiasSlopeFactor(0.0f)
	, lineWidth(0)
{

}

bool RasterizerDesc::operator ==(const RasterizerDesc& other) const
{
	return
		this->fillMode == other.fillMode &&
		this->cullMode == other.cullMode &&
		this->frontFace == other.frontFace &&
		this->discardEnable == other.discardEnable &&
		this->depthClampEnable == other.depthClampEnable &&
		this->depthBiasEnable == other.depthBiasEnable &&
		this->depthBiasConstantFactor == other.depthBiasConstantFactor &&
		this->depthBiasClamp == other.depthBiasClamp &&
		this->depthBiasSlopeFactor == other.depthBiasSlopeFactor &&
		this->lineWidth == other.lineWidth;
}

MultisampleDesc::MultisampleDesc()
{

}

bool MultisampleDesc::operator ==(const MultisampleDesc& other) const
{
	return
		this->rasterizationSamples == other.rasterizationSamples &&
		this->sampleShadingEnable == other.sampleShadingEnable &&
		this->minSampleShading == other.minSampleShading &&
		this->sampleMask == other.sampleMask &&
		this->alphaToCoverageEnable == other.alphaToCoverageEnable &&
		this->alphaToOneEnable == other.alphaToOneEnable;
}

StencilOpDesc::StencilOpDesc()
	: failOp(STENCIL_OP_KEEP)
	, passOp(STENCIL_OP_KEEP)
	, depthFailOp(STENCIL_OP_KEEP)
	, compareOp(CMP_OP_ALWAYS)
	, compareMask(0)
	, writeMask(0)
	, reference(0)
{

}

bool StencilOpDesc::operator ==(const StencilOpDesc& other) const
{
	return
		failOp == other.failOp &&
		passOp == other.passOp &&
		depthFailOp == other.depthFailOp &&
		compareOp == other.compareOp &&
		compareMask == other.compareMask &&
		writeMask == other.writeMask &&
		reference == other.reference;
}

DepthStencilDesc::DepthStencilDesc()
	: depthTestEnable(true)
	, depthWriteEnable(true)
	, depthBoundsTestEnable(false)
	, depthCompareOp(CMP_OP_LESS)
	, stencilTestEnable(false)
	, minDepthBounds(0.0f)
	, maxDepthBounds(1.0f)
{

}

bool DepthStencilDesc::operator ==(const DepthStencilDesc& other) const
{
	return
		depthTestEnable == other.depthTestEnable &&
		depthWriteEnable == other.depthWriteEnable &&
		depthBoundsTestEnable == other.depthBoundsTestEnable &&
		stencilTestEnable == other.stencilTestEnable &&
		front == other.front &&
		back == other.back &&
		minDepthBounds == other.minDepthBounds &&
		maxDepthBounds == other.maxDepthBounds;
}

BlendTargetDesc::BlendTargetDesc()
	: blendEnable(false)
	, srcColorFactor(BLEND_FACTOR_ONE)
	, dstColorFactor(BLEND_FACTOR_ZERO)
	, colorOp(BLEND_OP_ADD)
	, srcAlphaFactor(BLEND_FACTOR_ONE)
	, dstAlphaFactor(BLEND_FACTOR_ZERO)
	, alphaOp(BLEND_OP_ADD)
	, colorWriteMask(COLOR_MASK_ALL)
{

}

bool BlendTargetDesc::operator ==(const BlendTargetDesc& other) const
{
	return
		blendEnable == other.blendEnable &&
		srcColorFactor == other.srcColorFactor &&
		dstColorFactor == other.dstColorFactor &&
		colorOp == other.colorOp &&
		srcAlphaFactor == other.srcAlphaFactor &&
		dstAlphaFactor == other.dstAlphaFactor &&
		alphaOp == other.alphaOp &&
		colorWriteMask == other.colorWriteMask;
}

BlendDesc::BlendDesc()
	: logicOpEnable(false)
	, logicOp(LOGIC_OP_CLEAR)
{
	blendConstants[0] = 0.0f;
	blendConstants[1] = 0.0f;
	blendConstants[2] = 0.0f;
	blendConstants[3] = 0.0f;
}

bool BlendDesc::operator ==(const BlendDesc& other) const
{
	if (logicOpEnable != other.logicOpEnable ||
		logicOp != other.logicOp)
		return false;

	if (memcmp(blendConstants, other.blendConstants, sizeof(blendConstants)) != 0)
		return false;

	for (int i = 0; i < MAX_BLEND_TARGET; ++i)
	{
		if (targets[i] != other.targets[i])
			return false;
	}
	return true;
}

InputElement::InputElement(Semantic sem, PixelFormat format, uint8_t slot /* = 0 */, InputRate rate)
	:semantic(sem), format(format), slot(slot), offset(0), rate(rate)
{
}

CU_NS_END
