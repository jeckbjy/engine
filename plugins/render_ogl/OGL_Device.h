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
	RenderTarget*	newRenderWindow(Window* wnd);
	RenderTarget*	newRenderTexture(Texture* color, Texture* depth_stencil);
	InputLayout*	newInputLayout(const InputElement* elements, size_t count);
	ShaderStage*		newProgram();
	DescriptorSet*	newDescriptorSet(Pipeline* pipeline);
	Pipeline*		newPipeline(const PipelineDesc& desc);
	CommandBuffer*	newCommandBuffer();
	CommandQueue*	newCommandQueue();

	OGL_Program* getProgramPipeline();
private:

};

CU_NS_END