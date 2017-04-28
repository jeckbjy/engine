#pragma once
#include "OGL_API.h"

CUTE_NS_BEGIN

class CU_OGL_API OGL_FrameBuffer : public FrameBuffer
{
public:
	OGL_FrameBuffer();
	~OGL_FrameBuffer();

	void bind(void*);

private:
	void update();

private:
	GLuint m_fbo;
};

CUTE_NS_END
