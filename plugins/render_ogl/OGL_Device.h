#pragma once
#include "OGL_API.h"

CU_NS_BEGIN

class CU_OGL_API OGLDevice : public Device
{
public:
	OGLDevice();
	~OGLDevice();

	const String& name() const { return _name; }
	virtual bool init();

	GpuBuffer*		newBuffer(const BufferDesc& desc);
	Texture*		newTexture(const TextureDesc& desc);
	RenderTarget*	newRenderWindow(Window* wnd);
	RenderTarget*	newRenderTexture(Texture* color, Texture* depth_stencil);
	VertexLayout*	newVertexLayout(VertexDeclaration& desc);
	Program*		newProgram();
	DescriptorSet*	newDescriptorSet(Program* prog);
	Pipeline*		newPipeline(const PIPELINE_DESC& desc);
	CommandBuffer*	newCommandBuffer();
	CommandQueue*	newCommandQueue();

	void execute(CommandBuffer* cmd);
protected:
	String _name;
};

CU_NS_END