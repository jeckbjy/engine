#pragma once
#include "OGL_API.h"

CUTE_NS_BEGIN

class OGL_Program;
class CU_OGL_API OGL_Device : public Device
{
public:
	OGL_Device();
	~OGL_Device();

	GpuBuffer*		newBuffer(const BufferDesc& desc);
	Texture*		newTexture(const TextureDesc& desc);
	VertexArray*	newVertexArray(VertexLayout* layout);
	ShaderStage*	newShader();
	ShaderProgram*	newProgram();
	Pipeline*		newPipeline(const PipelineDesc& desc);
	DescriptorSet*	newDescriptorSet(Pipeline* pipeline);
	CommandBuffer*	newCommandBuffer();
	CommandQueue*	newCommandQueue();
	FrameBuffer*	newFrameBuffer();
	SwapChain*		newSwapChain(const SwapChainDesc& desc);

private:

};

CUTE_NS_END
