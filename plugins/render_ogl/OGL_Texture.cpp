#include "OGL_Texture.h"
#include "OGL_Mapping.h"

CU_NS_BEGIN

// 压缩，非压缩，数组，非数组共4种情况
static const char TEX_NORMAL = 0;
static const char TEX_ARRAYS = 1;
static const char TEX_COMPRESS = 2;
static const char TEX_COMPRESS_ARRAY = 3;

GLenum OGLTexture::getGLTarget(TexType type, uint32_t arrays)
{
	switch (type)
	{
	case TEX_1D:return arrays > 1 ? GL_TEXTURE_1D_ARRAY : GL_TEXTURE_1D;
	case TEX_2D:return arrays > 1 ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D;
	case TEX_3D:return GL_TEXTURE_3D;
	case TEX_CUBE:return arrays > 1 ? GL_TEXTURE_CUBE_MAP_ARRAY : GL_TEXTURE_CUBE_MAP;
	}
	return GL_TEXTURE_2D;
}
// todo:数据初始化
OGLTexture::OGLTexture(const TEXTURE_DESC& desc)
:Texture(desc)
{
	m_target = getGLTarget(desc.type, desc.depth);
	// 创建
	glGenTextures(1, &m_handle);
	glBindTexture(m_target, m_handle);
	// This needs to be set otherwise the texture doesn't get rendered
	glTexParameteri(m_target, GL_TEXTURE_MAX_LEVEL, desc.mipmaps - 1);
	// Set some misc default parameters so NVidia won't complain, these can of course be changed later
	glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	GLint glinternal;
	GLenum glformat;
	GLenum gltype;
	OGLMapping::getPixelFormat(desc.format, glinternal, glformat, gltype);

	// 分配空间
	bool compressed = PixelUtil::isCompressed(m_format);

	char fill_mode;
	if (compressed)
		fill_mode = m_depth > 1 ? TEX_COMPRESS_ARRAY : TEX_COMPRESS;
	else
		fill_mode = m_depth > 1 ? TEX_ARRAYS : TEX_NORMAL;

	if ((m_usage & RES_RENDER_TARGET) || (m_usage & RES_DEPTH_STENCIL))
	{
		if (desc.samples > 0)
			glTexImage2DMultisample(GL_TEXTURE_2D, desc.samples, glinternal, m_width, m_height, GL_FALSE);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, glinternal, m_width, m_height, 0, glformat, gltype, NULL);
	}
	const char* data = (const char*)desc.data;
	// 2d
	switch (m_type)
	{
	case TEX_1D:
		create1D(glinternal, glformat, gltype, compressed, fill_mode, data);
		break;
	case TEX_2D:
		create2D(glinternal, glformat, gltype, compressed, fill_mode, data);
		break;
	case TEX_3D:
		create3D(glinternal, glformat, gltype, compressed, fill_mode, data);
		break;
	case TEX_CUBE:
		createCube(glinternal, glformat, gltype, compressed, fill_mode, data);
		break;
	default:
		break;
	}
}

OGLTexture::~OGLTexture()
{
	glDeleteTextures(1, &m_handle);
}

void OGLTexture::active(GLint index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(m_target, m_handle);
}

void* OGLTexture::map(PixelData& data, MAP_FLAG flag, uint level, uint face)
{
	return 0;
}

void OGLTexture::unmap()
{

}

void OGLTexture::read(PixelData& data, uint level, uint face)
{
	// 传入参数不用这么复杂？？
	glBindTexture(m_target, m_handle);
	GLenum target = (m_type == TEX_CUBE) ? GL_TEXTURE_CUBE_MAP_POSITIVE_X + face : m_target;
	if (PixelUtil::isCompressed(m_format))
	{// 创建空间？？
		glGetCompressedTexImage(target, level, data.data);
	}
	else
	{
		glGetTexImage(target, level, OGLMapping::getGLFormat(m_format), OGLMapping::getGLType(m_format), data.data);
	}
}

void OGLTexture::write(const PixelData& data, uint level, uint face, bool discard)
{
	// 还能再简化么？
	glBindTexture(m_target, m_handle);
	GLint glinternal;
	GLenum glformat;
	GLenum gltype;
	OGLMapping::getPixelFormat(m_format, glinternal, glformat, gltype);

	uint image_size = 0;
	bool compressed = PixelUtil::isCompressed(m_format);
	switch (m_target)
	{
	case GL_TEXTURE_1D:
		if (compressed)
			glCompressedTexSubImage1D(m_target, level, data.x, data.width, glformat, image_size, data.data);
		else
			glTexSubImage1D(m_target,level,data.x, data.width, glformat, gltype, data.data);
		break;
	case GL_TEXTURE_2D:
	case GL_TEXTURE_CUBE_MAP:
	{
		GLenum target = (m_type == TEX_CUBE) ? GL_TEXTURE_CUBE_MAP_POSITIVE_X + face : GL_TEXTURE_2D;
		if (compressed)
			glCompressedTexSubImage2D(target, level, data.x, data.y, data.width, data.height, glformat, image_size, data.data);
		else
			glTexSubImage2D(target, level, data.x, data.y, data.width, data.height, glformat, gltype, data.data);
	}
		break;
	case GL_TEXTURE_3D:
		if (compressed)
			glCompressedTexSubImage3D(m_target, level, data.x, data.y, data.z, data.width, data.height, data.depth, glformat, image_size, data.data);
		else
			glTexSubImage3D(m_target, level, data.x, data.y, data.z, data.width, data.height, data.depth, glformat, gltype, data.data);
		break;
	default:
		break;
	}
}

void OGLTexture::create1D(GLint glinternal, GLenum glformat, GLenum gltype, bool compressed, char fill_mode, const char* data)
{
	size_t block_size = PixelUtil::getBytes(m_format);

	size_t width, image_size;
	for (uint32_t array_index = 0; array_index < m_depth; ++array_index)
	{
		width = m_width;
		for (uint32_t level = 0; level < m_mipmaps; ++level)
		{
			if (compressed)
				image_size = ((width + 3) / 4) * block_size;
			else
				image_size = width * block_size;

			switch (fill_mode)
			{
			case TEX_NORMAL:
				glTexImage1D(m_target, level, glinternal, width, 0, glformat, gltype, data);
				break;
			case TEX_COMPRESS:
				glCompressedTexImage1D(m_target, level, glinternal, width, 0, image_size, data);
				break;
			case TEX_ARRAYS:
				glTexImage2D(m_target, level, glinternal, width, m_depth, 0, glformat, gltype, data);
				break;
			case TEX_COMPRESS_ARRAY:
				glCompressedTexImage2D(m_target, level, glinternal, width, m_depth, 0, image_size, data);
			default:
				break;
			}
			if (width > 1)
				width >>= 1;
			if (data != NULL)
				data += image_size;
		}
	}
}

void OGLTexture::create2D(GLint glinternal, GLenum glformat, GLenum gltype, bool compressed, char fill_mode, const char* data)
{
	size_t block_size = PixelUtil::getBytes(m_format);

	size_t width, height, image_size;
	for (uint32_t array_index = 0; array_index < m_depth; ++array_index)
	{
		width	= m_width;
		height	= m_height;
		for (uint32_t level = 0; level < m_mipmaps; ++level)
		{
			if (compressed)
				image_size = ((width + 3) / 4) * ((height + 3) / 4)*block_size;
			else
				image_size = width * height * block_size;
			switch (fill_mode)
			{
			case TEX_NORMAL:
				glTexImage2D(m_target, level, glinternal, width, height, 0, glformat, gltype, data);
				break;
			case TEX_COMPRESS:
				glCompressedTexImage2D(m_target, level, glinternal, width, height, 0, image_size, data);
				break;
			case TEX_ARRAYS:
				glTexImage3D(m_target, level, glinternal, width, height, m_depth, 0, glformat, gltype, data);
				break;
			case TEX_COMPRESS_ARRAY:
				glCompressedTexImage3D(m_target, level, glinternal, width, height, m_depth, 0, image_size, data);
			default:
				break;
			}
			//
			if (width > 1)
				width >>= 1;
			if (height > 1)
				height >>= 1;
			if (data != NULL)
				data += image_size;
		}
	}
}

void OGLTexture::create3D(GLint glinternal, GLenum glformat, GLenum gltype, bool compressed, char fill_mode, const char* data)
{
	size_t block_size = PixelUtil::getBytes(m_format);

	size_t width, height, depth, image_size;
	width	= m_width;
	height	= m_height;
	depth	= m_depth;
	for (uint32_t level = 0; level < m_mipmaps; ++level)
	{
		if (compressed)
		{
			image_size = ((width + 3) / 4) * ((height + 3) / 4) * depth *block_size;
			glCompressedTexImage3D(m_target, level, glinternal, width, height, depth, 0, image_size, data);
		}
		else
		{
			image_size = width * height * depth * block_size;
			glTexImage3D(m_target, level, glinternal, width, height, depth, 0, glformat, gltype, data);
		}
		if (width > 1)
			width >>= 1;
		if (height > 1)
			height >>= 1;
		if (depth > 1)
			depth >>= 1;
		if (data != NULL)
			data += image_size;
	}
}

void OGLTexture::createCube(GLint glinternal, GLenum glformat, GLenum gltype, bool compressed, char fill_mode, const char* data)
{
	size_t block_size = PixelUtil::getBytes(m_format);
	size_t width, height, image_size;
	
	GLenum target = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	// can change to: _depth * 6 ?
	for (uint32_t array_index = 0; array_index < m_depth; ++array_index)
	{
		for (uint32_t face = 0; face < 6; ++face)
		{
			width	= m_width;
			height	= m_height;
			for (uint32_t level = 0; level < m_mipmaps; ++level)
			{
				if (compressed)
					image_size = ((width + 3) / 4) * ((height + 3) / 4)*block_size;
				else
					image_size = width * height * block_size;
				switch (fill_mode)
				{
				case TEX_NORMAL:
					glTexImage2D(target + face, level, glinternal, width, height, 0, glformat, gltype, data);
					break;
				case TEX_COMPRESS:
					glCompressedTexImage2D(target + face, level, glinternal, width, height, 0, image_size, data);
					break;
				case TEX_ARRAYS:
					glTexImage3D(target + face, level, glinternal, width, height, m_depth, 0, glformat, gltype, data);
					break;
				case TEX_COMPRESS_ARRAY:
					glCompressedTexImage3D(target + face, level, glinternal, width, height, m_depth, 0, image_size, data);
				default:
					break;
				}
				//
				if (width > 1)
					width >>= 1;
				if (height > 1)
					height >>= 1;
				if (data != NULL)
					data += image_size;
			}
		}
	}
}

CU_NS_END