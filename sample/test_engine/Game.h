#pragma once
#include "Matrix4.h"
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

	virtual bool setup();
	virtual bool init();
	virtual void update();
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
	Matrix4				m_worldMatrix;
	Matrix4				m_projMatrix;
	Matrix4				m_viewMatrix;
};

class TriangleApp : public BaseApp
{
public:
	bool init();
};
