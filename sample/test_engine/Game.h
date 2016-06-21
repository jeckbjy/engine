#pragma once
#include "Window.h"
#include "Graphics.h"
#include "Application.h"
using namespace cute;

// ≤‚ ‘ª˘¿‡
class BaseApp : public Application
{
public:
	BaseApp();
	~BaseApp();
	void run();

	virtual bool init(){ return true; }
	virtual void draw();

	 ShaderProgram* loadProgram(const char* vsFile, const char* psFile);
	 Pipeline* newPipeline(ShaderProgram* prog);

protected:
	typedef SharedPtr<Window>	WindowPtr;
	size_t				m_width;
	size_t				m_height;
	Device*				m_device;
	WindowPtr			m_window;
	CommandBufferPtr	m_cmdBuffer;
	CommandQueuePtr		m_cmdQueue;
	SwapChainPtr		m_swapchain;
	ShaderProgramPtr	m_program;
	PipelinePtr			m_pipeline;
	DescriptorSetPtr	m_descriptors;
	InputLayoutPtr		m_layout;
	GpuBufferPtr		m_vb;
	GpuBufferPtr		m_ib;
};

class TriangleApp : public BaseApp
{
public:
	bool init();
};

