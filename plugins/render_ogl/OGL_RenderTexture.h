#pragma once
#include "OGL_FrameBuffer.h"
#include <vector>

CU_NS_BEGIN

class OGLTexture;
class CU_OGL_API OGLRenderTexture : public OGLFrameBuffer
{
	typedef std::vector<OGLTexture*> TextureVec;
public:
	OGLRenderTexture(Texture* color, Texture* depth_stencil);
	~OGLRenderTexture();
	
	void present();

	void bind();
	void attach(uint32_t att, Texture* tex);
	void detach(uint32_t att);

protected:
	OGLTexture*	m_tex_ds;	// depth stencil
	TextureVec	m_colors;
	uint8_t		m_dirty;
};


CU_NS_END