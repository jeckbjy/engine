#pragma once
#include "OGL_FrameBuffer.h"
#include <vector>

CU_NS_BEGIN

class OGL_Texture;
class CU_OGL_API OGL_RenderTexture : public OGL_FrameBuffer
{
	typedef std::vector<OGL_Texture*> TextureVec;
public:
	OGL_RenderTexture(Texture* color, Texture* depth_stencil);
	~OGL_RenderTexture();
	
	void present();

	void bind();
	void attach(uint32_t att, Texture* tex);
	void detach(uint32_t att);

protected:
	OGL_Texture*	m_tex_ds;	// depth stencil
	TextureVec	m_colors;
	uint8_t		m_dirty;
};


CU_NS_END