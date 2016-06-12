#pragma once
#include "OGL_API.h"

CU_NS_BEGIN

class CU_OGL_API OGL_Texture : public Texture
{
	static GLenum getGLTarget(TexType type, uint32_t arrays);
public:
	OGL_Texture(const TextureDesc& desc);
	virtual ~OGL_Texture();

	void* map(PixelData& data, MAP_FLAG flag, uint level, uint face);
	void unmap();

	void read(PixelData& data, uint level, uint face);
	void write(const PixelData& data, uint level, uint face, bool discard);

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
	GLenum m_target;	// 1D,2D,3DµÈ
};

CU_NS_END