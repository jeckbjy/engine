#include "OGL_FrameBuffer.h"

CU_NS_BEGIN

GLuint OGLFrameBuffer::s_cur_fbo = 0;

bool OGLFrameBuffer::bindFBO(GLuint fbo)
{
	if (fbo != s_cur_fbo)
	{
		s_cur_fbo = fbo;
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		return true;
	}
	return false;
}

OGLFrameBuffer::OGLFrameBuffer(bool off_screen)
:m_fbo(0)
{
	// 离线渲染
	if (off_screen)
		glGenFramebuffers(1, &m_fbo);
}

OGLFrameBuffer::~OGLFrameBuffer()
{
	if (m_fbo != 0)
	{
		glDeleteFramebuffers(1, &m_fbo);
		m_fbo = 0;
	}
}

void OGLFrameBuffer::clear(uint32_t flags, const Color& color, float depth, int32_t stencil)
{
	bindFBO(m_fbo);
	// 需要根据当前的BlendState和DepthStencilState设置
	if (flags & CLEAR_COLOR)
	{// 更新颜色
		glColorMask(true, true, true, true);
		glClearColor(color.r, color.g, color.b, color.a);
	}

	if (flags & CLEAR_DEPTH)
	{
		glDepthMask(GL_TRUE);
		glClearDepth(depth);
	}

	if (flags & CLEAR_STENCIL)
	{
		glStencilMask(0xFFFFFFFF);
		glClearStencil(stencil);
	}
}

void OGLFrameBuffer::discard(uint32_t flags)
{

}

void OGLFrameBuffer::resize(uint32_t width, uint32_t height)
{

}

CU_NS_END