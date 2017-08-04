#include "OGL_Texture.h"
#include "OGL_Mapping.h"

CUTE_NS_BEGIN

static const char TEX_NORMAL = 0;
static const char TEX_ARRAYS = 1;
static const char TEX_COMPRESS = 2;
static const char TEX_COMPRESS_ARRAY = 3;

GLenum OGL_Texture::getGLTarget(RESOURCE_DIMENSION type, uint32_t arrays)
{
	switch (type)
	{
	case RESOURCE_DIMENSION_TEXTURE1D:
        return arrays > 1 ? GL_TEXTURE_1D_ARRAY : GL_TEXTURE_1D;
	case RESOURCE_DIMENSION_TEXTURE2D:
        return arrays > 1 ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D;
	case RESOURCE_DIMENSION_TEXTURE3D:
        return GL_TEXTURE_3D;
	case RESOURCE_DIMENSION_CUBEMAP:
        return arrays > 1 ? GL_TEXTURE_CUBE_MAP_ARRAY : GL_TEXTURE_CUBE_MAP;
    default:
        assert(false);
        break;
	}
    
	return GL_TEXTURE_2D;
}

OGL_Texture::OGL_Texture(const TextureDesc& desc)
    : ITexture(desc)
    , m_handle(0)
    , m_target(0)
{
}

OGL_Texture::~OGL_Texture()
{
    term();
}

bool OGL_Texture::init(const TextureDesc& desc)
{
    m_target = getGLTarget(desc.dimension, desc.depthOrArraySize);
    //
    glGenTextures(1, &m_handle);
    glBindTexture(m_target, m_handle);
    // This needs to be set otherwise the texture doesn't get rendered
    glTexParameteri(m_target, GL_TEXTURE_MAX_LEVEL, desc.mipLevels - 1);
    // Set some misc default parameters so NVidia won't complain, these can of course be changed later
    glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    GLint  glinternal;
    GLenum glformat;
    GLenum gltype;
    OGL_Mapping::getFormat(desc.format, glinternal, glformat, gltype);
    
    //
    bool compressed = PixelUtil::isCompressed(desc.format);
    
    char fill_mode;
    if (compressed)
        fill_mode = desc.depthOrArraySize > 1 ? TEX_COMPRESS_ARRAY : TEX_COMPRESS;
    else
        fill_mode = desc.depthOrArraySize > 1 ? TEX_ARRAYS : TEX_NORMAL;
    
    if ((desc.usage & RESOURCE_USAGE_COLOR_TARGET) || (desc.usage & RESOURCE_USAGE_DEPTH_TARGET))
    {
        if (desc.sampleCount > 0)
            glTexImage2DMultisample(GL_TEXTURE_2D, desc.sampleCount, glinternal, desc.width, desc.height, GL_FALSE);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, glinternal, desc.width, desc.height, 0, glformat, gltype, NULL);
    }
    
    const char* data = (const char*)desc.data;
    // 2d
    switch (desc.dimension)
    {
        case RESOURCE_DIMENSION_TEXTURE1D:
            create1D(glinternal, glformat, gltype, compressed, fill_mode, data);
            break;
        case RESOURCE_DIMENSION_TEXTURE2D:
            create2D(glinternal, glformat, gltype, compressed, fill_mode, data);
            break;
        case RESOURCE_DIMENSION_TEXTURE3D:
            create3D(glinternal, glformat, gltype, compressed, fill_mode, data);
            break;
        case RESOURCE_DIMENSION_CUBEMAP:
            createCube(glinternal, glformat, gltype, compressed, fill_mode, data);
            break;
        default:
            break;
    }

    return true;
}

void OGL_Texture::term()
{
    if(m_handle != 0)
    {
        glDeleteTextures(1, &m_handle);
        m_handle = 0;
    }
}

void OGL_Texture::active(GLint index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(m_target, m_handle);
}

void OGL_Texture::bindToFrameBuffer(GLenum attachment)
{
	switch (m_target)
	{
	case GL_TEXTURE_1D:
		glFramebufferTexture1D(GL_FRAMEBUFFER, attachment, 0, m_handle, 0);
		break;
	case GL_TEXTURE_2D:
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, 0, m_handle, 0);
		break;
	case GL_TEXTURE_2D_MULTISAMPLE:
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, 0, m_handle, 0);
		break;
    case GL_TEXTURE_3D:
	case GL_TEXTURE_CUBE_MAP:
	default:
		glFramebufferTexture(GL_FRAMEBUFFER, attachment, m_handle, 0);
		break;
	}
}

bool OGL_Texture::map(void_ptr& result)
{
	return false;
}

void OGL_Texture::unmap()
{

}

//void OGL_Texture::read(PixelData& data, uint level, uint face)
//{
//	// ¥´»Î≤Œ ˝≤ª”√’‚√¥∏¥‘”£ø£ø
//	glBindTexture(m_target, m_handle);
//	GLenum target = (m_type == TEX_CUBE) ? GL_TEXTURE_CUBE_MAP_POSITIVE_X + face : m_target;
//	if (PixelUtil::isCompressed(m_format))
//	{// ¥¥Ω®ø’º‰£ø£ø
//		glGetCompressedTexImage(target, level, data.data);
//	}
//	else
//	{
//		glGetTexImage(target, level, OGL_Mapping::getGLFormat(m_format), OGL_Mapping::getGLType(m_format), data.data);
//	}
//}
//
//void OGL_Texture::write(const PixelData& data, uint level, uint face, bool discard)
//{
//	// ªπƒ‹‘ŸºÚªØ√¥£ø
//	glBindTexture(m_target, m_handle);
//	GLint glinternal;
//	GLenum glformat;
//	GLenum gltype;
//	OGL_Mapping::getPixelFormat(m_format, glinternal, glformat, gltype);
//
//	uint image_size = 0;
//	bool compressed = PixelUtil::isCompressed(m_format);
//	switch (m_target)
//	{
//	case GL_TEXTURE_1D:
//		if (compressed)
//			glCompressedTexSubImage1D(m_target, level, data.x, data.width, glformat, image_size, data.data);
//		else
//			glTexSubImage1D(m_target,level,data.x, data.width, glformat, gltype, data.data);
//		break;
//	case GL_TEXTURE_2D:
//	case GL_TEXTURE_CUBE_MAP:
//	{
//		GLenum target = (m_type == TEX_CUBE) ? GL_TEXTURE_CUBE_MAP_POSITIVE_X + face : GL_TEXTURE_2D;
//		if (compressed)
//			glCompressedTexSubImage2D(target, level, data.x, data.y, data.width, data.height, glformat, image_size, data.data);
//		else
//			glTexSubImage2D(target, level, data.x, data.y, data.width, data.height, glformat, gltype, data.data);
//	}
//		break;
//	case GL_TEXTURE_3D:
//		if (compressed)
//			glCompressedTexSubImage3D(m_target, level, data.x, data.y, data.z, data.width, data.height, data.depth, glformat, image_size, data.data);
//		else
//			glTexSubImage3D(m_target, level, data.x, data.y, data.z, data.width, data.height, data.depth, glformat, gltype, data.data);
//		break;
//	default:
//		break;
//	}
//}

void OGL_Texture::create1D(GLint glinternal, GLenum glformat, GLenum gltype, bool compressed, char fill_mode, const char* data)
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

void OGL_Texture::create2D(GLint glinternal, GLenum glformat, GLenum gltype, bool compressed, char fill_mode, const char* data)
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

void OGL_Texture::create3D(GLint glinternal, GLenum glformat, GLenum gltype, bool compressed, char fill_mode, const char* data)
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

void OGL_Texture::createCube(GLint glinternal, GLenum glformat, GLenum gltype, bool compressed, char fill_mode, const char* data)
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

CUTE_NS_END
