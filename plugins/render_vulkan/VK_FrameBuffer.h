#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class CU_VK_API VK_FrameBuffer : public FrameBuffer
{
public:
	VK_FrameBuffer();
	~VK_FrameBuffer();

	void bind();

private:
	void destroy();
	void update();

private:
	VkFramebuffer m_fbo;
};

CU_NS_END