#include "OGL_Device.h"
#include "OGL_Mapping.h"
#include "OGL_Program.h"
#include "OGL_Texture.h"
#include "OGL_Buffer.h"
#include "OGL_CommandBuffer.h"
#include "OGL_Pipeline.h"
#include "OGL_DescriptorSet.h"
#include "OGL_FrameBuffer.h"
#include "OGL_SwapChain.h"
#include "OGL_VertexArray.h"
#include "Window.h"

CU_NS_BEGIN

OGL_Device::OGL_Device()
{

}

OGL_Device::~OGL_Device()
{

}

GpuBuffer* OGL_Device::newBuffer(const BufferDesc& desc)
{
	return new OGL_Buffer(desc);
}

Texture* OGL_Device::newTexture(const TextureDesc& desc)
{
	return new OGL_Texture(desc);
}

DescriptorSet* OGL_Device::newDescriptorSet(Pipeline* pipeline)
{
	return new OGL_DescriptorSet(pipeline);
}

VertexLayout* OGL_Device::newVertexLayout(const VertexElement* elements, size_t count)
{
	return new VertexLayout(elements, count);
}

VertexArray* OGL_Device::newVertexArray(VertexLayout* layout)
{
	return new OGL_VertexArray(layout);
}

ShaderStage* OGL_Device::newShader()
{
	return new OGL_Shader();
}

ShaderProgram* OGL_Device::newProgram()
{
	return new OGL_Program();
}

Pipeline* OGL_Device::newPipeline(const PipelineDesc& desc)
{
	return new OGL_Pipeline(desc);
}

CommandBuffer* OGL_Device::newCommandBuffer()
{
	return new OGL_CommandBuffer();
}

CommandQueue* OGL_Device::newCommandQueue()
{
	return new OGLCommandQueue();
}

FrameBuffer* OGL_Device::newFrameBuffer()
{
	return new OGL_FrameBuffer();
}

SwapChain* OGL_Device::newSwapChain(const SwapChainDesc& desc)
{
	return new OGL_SwapChain(desc);
}

CU_NS_END
