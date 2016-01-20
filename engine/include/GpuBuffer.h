#pragma once
#include "GraphicsDef.h"

CU_NS_BEGIN

class CU_API GpuBuffer : public GpuResource
{
	DECLARE_RTTI(GpuBuffer, GpuResource, "GBUF");
public:
	virtual ~GpuBuffer(){}
	virtual void* map(MAP_FLAG access, size_t offset, size_t len) = 0;
	virtual void  unmap() = 0;
};

typedef GpuBuffer VertexBuffer;
typedef GpuBuffer IndexBuffer;
typedef GpuBuffer UniformBuffer;

CU_NS_END