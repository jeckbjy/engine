#pragma once
#include "GraphicsDef.h"
#include "PixelFormat.h"

CU_NS_BEGIN

struct CU_API TextureDesc
{
	TexType		type;
	ResFlag		usage;
	PixelFormat format;
	uint32_t	width;
	uint32_t	height;
	uint32_t	depth;	// or array size
	uint32_t	mipmaps;
	uint32_t	samples;
	uint32_t	quality;
	void*		data;
	TextureDesc();
	TextureDesc(PixelFormat format, uint32_t width, uint32_t height, void* data = NULL, ResFlag flags = RES_DEFAULT);
};

class CU_API Texture : public GpuResource
{
	DECLARE_RTTI(Texture, GpuResource, "GTEX");
public:
	Texture(const TextureDesc& desc);
	virtual ~Texture(){}

	virtual void map(PixelData& data, MAP_FLAG flag, uint level = 0, uint face = 0) = 0;
	virtual void unmap() = 0;
};

CU_NS_END
