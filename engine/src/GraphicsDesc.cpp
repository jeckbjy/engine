#include "GraphicsDesc.h"

CU_NS_BEGIN

TextureDesc::TextureDesc()
{

}

TextureDesc::TextureDesc(PixelFormat fmt, uint32_t width, uint32_t height, TextureUsage usage)
{

}

SamplerDesc::SamplerDesc()
{

}

RasterizerDesc::RasterizerDesc()
{

}

MultisampleDesc::MultisampleDesc()
{

}

StencilOpState::StencilOpState()
{

}

DepthStencilDesc::DepthStencilDesc()
{

}

BlendTargetDesc::BlendTargetDesc()
{

}

BlendDesc::BlendDesc()
{

}

InputElement::InputElement(Semantic sem, PixelFormat format, uint8_t slot /* = 0 */, InputRate rate)
	:semantic(sem), format(format), slot(slot), offset(0), rate(rate)
{
}

CU_NS_END
