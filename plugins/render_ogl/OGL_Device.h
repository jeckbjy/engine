#pragma once
#include "OGL_API.h"

CU_NS_BEGIN

class OGL_Program;
class CU_OGL_API OGL_Device : public Device
{
public:
	OGL_Device();
	~OGL_Device();

	virtual bool init();

	GpuBuffer*		newBuffer(const BufferDesc& desc);
	Texture*		newTexture(const TextureDesc& desc);
	InputLayout*	newInputLayout(const InputElement* elements, size_t count);
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

CU_NS_END