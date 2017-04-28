#pragma once
#include "VK_API.h"

CUTE_NS_BEGIN

class CU_VK_API VK_FrameBuffer : public FrameBuffer
{
public:
	VK_FrameBuffer();
	~VK_FrameBuffer();

	void bind(void*);

private:
	void destroy();
	void update();

private:
	VkFramebuffer m_fbo;
};

CUTE_NS_END
