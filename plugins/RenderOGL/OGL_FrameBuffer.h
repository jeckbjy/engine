#pragma once
#include "OGL_API.h"

CUTE_NS_BEGIN

class OGL_Texture;
class CUTE_OGL_API OGL_FrameBuffer : public IFrameBuffer
{
public:
	OGL_FrameBuffer();
	~OGL_FrameBuffer();

	void bind(void*);

private:
	void update();

private:
    typedef Vector<OGL_Texture*> TextureList;
	GLuint      m_fbo;
    bool        m_dirty;
    TextureList m_attachments;
};

CUTE_NS_END
