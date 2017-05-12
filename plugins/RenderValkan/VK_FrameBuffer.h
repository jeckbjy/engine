#pragma once
#include "VK_API.h"

CUTE_NS_BEGIN

class VK_Device;
class CUTE_VK_API VK_FrameBuffer : public IFrameBuffer
{
public:
	VK_FrameBuffer();
	~VK_FrameBuffer();

	bool init(VK_Device* device, const FrameBufferDesc& desc);
	void term();

	void bind(ICommandList* cmdList);
	void clear(
		ICommandList*					cmdList,
		uint32_t						clearColorCount,
		const ClearColorValue*			clearColors,
		const ClearDepthStencilValue*	clearDepthStencil);

private:
	VK_Device*		m_device;
	VkFramebuffer	m_buffer;
	VkRenderPass	m_pass;
};

CUTE_NS_END
