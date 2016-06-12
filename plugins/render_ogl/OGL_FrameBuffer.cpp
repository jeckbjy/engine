#include "OGL_FrameBuffer.h"
#include "OGL_Texture.h"

CU_NS_BEGIN

OGL_FrameBuffer::OGL_FrameBuffer()
	: m_fbo(0)
{
	glGenFramebuffers(1, &m_fbo);
}

OGL_FrameBuffer::~OGL_FrameBuffer()
{
	if (m_fbo != 0)
	{
		glDeleteFramebuffers(1, &m_fbo);
		m_fbo = 0;
	}
}

void OGL_FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	if (m_dirty)
		update();
}

void OGL_FrameBuffer::update()
{
	m_dirty = false;
	if (m_colors.empty())
		return;

	OGL_Texture* ds = (OGL_Texture*)(m_depthStencil.get());
	if (ds)
	{
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, ds->handle());
	}

	OGL_Texture* tex;
	GLenum attachments[8];
	GLsizei maxSize = 0;
	for (size_t i = 0; i < m_colors.size(); ++i)
	{
		tex = (OGL_Texture*)(m_colors[i].get());
		if (tex)
		{
			tex->bindToFrameBuffer(GL_COLOR_ATTACHMENT0 + i);
			attachments[i] = GL_COLOR_ATTACHMENT0 + i;
			maxSize = i + 1;
		}
		else
		{
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, 0, 0);
			attachments[i] = GL_NONE;
		}
	}
	glDrawBuffers(maxSize, attachments);
}

CU_NS_END
