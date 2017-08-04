#pragma once
#include "OGL_API.h"

CUTE_NS_BEGIN

class CUTE_OGL_API OGL_Texture : public ITexture
{
	static GLenum getGLTarget(RESOURCE_DIMENSION type, uint32_t arrays);
public:
	OGL_Texture(const TextureDesc& desc);
	virtual ~OGL_Texture();
    
    bool init(const TextureDesc& desc);
    void term();

	bool map(void_ptr& reslut);
	void unmap();

	void active(GLint index);
	void bindToFrameBuffer(GLenum attachment);

	GLuint handle() const { return m_handle; }
	GLenum target() const { return m_target; }

private:
	void create1D(GLint glinternal, GLenum glformat, GLenum gltype, bool compressed, char fill_mode, const char* data);
	void create2D(GLint glinternal, GLenum glformat, GLenum gltype, bool compressed, char fill_mode, const char* data);
	void create3D(GLint glinternal, GLenum glformat, GLenum gltype, bool compressed, char fill_mode, const char* data);
	void createCube(GLint glinternal, GLenum glformat, GLenum gltype, bool compressed, char fill_mode, const char* data);

protected:
	GLuint m_handle;
	GLenum m_target;	// 1D,2D,3D
};

CUTE_NS_END
