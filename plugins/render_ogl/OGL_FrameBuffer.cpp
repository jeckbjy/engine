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

void OGL_FrameBuffer::bind(void*)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	if (m_dirty)
		update();
}

void OGL_FrameBuffer::update()
{
	m_dirty = false;
	if (m_attachments.empty())
		return;

	OGL_Texture* ds = (OGL_Texture*)(m_attachments[0].get());
	if (ds)
	{
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, ds->handle());
	}

	OGL_Texture* tex;
	GLenum attachments[8];
	GLsizei maxSize = 0;
	size_t index;
	for (size_t i = 1; i < m_attachments.size(); ++i)
	{
		index = i - 1;
		tex = (OGL_Texture*)(m_attachments[i].get());
		if (tex)
		{
			tex->bindToFrameBuffer(GL_COLOR_ATTACHMENT0 + index);
			attachments[index] = GL_COLOR_ATTACHMENT0 + index;
			maxSize = index + 1;
		}
		else
		{
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, 0, 0);
			attachments[index] = GL_NONE;
		}
	}
	glDrawBuffers(maxSize, attachments);
}

CU_NS_END
