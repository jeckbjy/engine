#pragma once
#include "OGL_API.h"

CU_NS_BEGIN

// glDrawBuffer:������ȾĿ��
// ����������Ⱦ,FrameBuffer
class CU_OGL_API OGL_FrameBuffer : public RenderTarget
{
protected:
	static GLuint s_cur_fbo;
	static bool bindFBO(GLuint fbo);
public:
	OGL_FrameBuffer(bool off_screen);
	~OGL_FrameBuffer();

	void clear(uint32_t flags, const Color& color, float depth, int32_t stencil);
	void discard(uint32_t flags);
	void resize(uint32_t width, uint32_t height);

protected:
	GLuint m_fbo;
};

CU_NS_END