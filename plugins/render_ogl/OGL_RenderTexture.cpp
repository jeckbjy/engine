#include "OGL_RenderTexture.h"
#include "OGL_Texture.h"

CU_NS_BEGIN

OGLRenderTexture::OGLRenderTexture(Texture* color, Texture* depth_stencil)
:OGLFrameBuffer(true)
, m_dirty(TRUE)
{
	if (color != NULL)
		m_colors.push_back((OGLTexture*)color);
	m_tex_ds = (OGLTexture*)depth_stencil;
}

OGLRenderTexture::~OGLRenderTexture()
{
	// 释放需要释放 texture ？？
}

void OGLRenderTexture::present()
{
	throw std::runtime_error("RenderTexture cannot use swapBuffers");
}


void OGLRenderTexture::bind()
{
	if (bindFBO(m_fbo) || m_dirty)
	{
		if (m_tex_ds != NULL)
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_tex_ds->handle());
		std::vector<GLenum> targets(m_colors.size());
		for (size_t i = 0; i < targets.size(); ++i)
		{// 需要清除NULL的么？？
			targets[i] = (GLenum)(GL_COLOR_ATTACHMENT0 + i);
		}
		glDrawBuffers(targets.size(), &targets[0]);
	}
}

void OGLRenderTexture::attach(uint32_t att, Texture* tex)
{
	OGLTexture* ogl_tex = (OGLTexture*)tex;
	if (att == ATT_DepthStencil)
	{
		m_tex_ds = ogl_tex;
	}
	else
	{
		uint32_t index = att - ATT_Color;
		if (m_colors.size() <= index)
		{
			m_colors.resize(index + 1);
		}
		m_colors[index] = ogl_tex;
	}
}

void OGLRenderTexture::detach(uint32_t att)
{
	if (att == ATT_DepthStencil)
	{
		m_tex_ds = NULL;
	}
	else
	{
		uint32_t index = att - ATT_Color;
		if (index < m_colors.size() && m_colors[index] != NULL)
		{
			m_colors[index] = NULL;
		}
	}
}

CU_NS_END