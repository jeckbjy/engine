#include "Game.h"
#include "AssetCache.h"
#include "Model.h"
#include "Engine.h"
#include "Util.h"

BaseApp::BaseApp()
	: m_width(800)
	, m_height(600)
{
}

BaseApp::~BaseApp()
{

}

bool BaseApp::init()
{
	return true;
}

bool BaseApp::setup()
{
	// load plugins
	const char* plugins[] = { "plugin_d3d11" };

	for (size_t i = 0; i < CU_ARRAY_SIZE(plugins); ++i)
	{
		gEngine.loadPlugin(plugins[i]);
	}

	m_device = gGraphics.getDevice();
	if (!m_device)
		return false;

	m_worldMatrix.set(1.0f);
	//m_projMatrix = Matrix4::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	m_projMatrix = Matrix4::perspectiveFov(Math::PI / 4.0f, m_width / m_height, 0.1f, 1000.0f);
	m_viewMatrix = Matrix4::lookAt(Vector3(0, 0, -10), Vector3(0,0,-9), Vector3::UNIT_Y);

	m_window = new Window(NULL, "Test", m_width, m_height);

	m_cmdBuffer = m_device->newCommandBuffer();
	m_cmdQueue = m_device->newCommandQueue();
	m_swapchain = m_device->newSwapChain(SwapChainDesc(m_window));
	m_cmdBuffer->setViewport(0, 0, m_window->getWidth(), m_window->getHeight());
	m_cmdBuffer->setRenderTarget(m_swapchain);

	if (!init())
		return false;

	return true;
}

void BaseApp::update()
{
	draw();
}

void BaseApp::draw()
{
	m_cmdBuffer->clear();
	m_cmdBuffer->drawIndexed(m_ib->count());
	m_swapchain->present();

	//if (m_cmdQueue)
	//	m_cmdQueue->submit(m_cmdBuffer, NULL);
}

ShaderProgram* BaseApp::loadProgram(const char* vsFile, const char* psFile)
{
	ShaderStage* vs = m_device->loadShader(SHADER_VERTEX, vsFile);
	ShaderStage* ps = m_device->loadShader(SHADER_PIXEL, psFile);
	ShaderProgram* prog = m_device->newProgram();
	prog->attach(vs);
	prog->attach(ps);
	prog->link();
	return prog;
}

Pipeline* BaseApp::newPipeline(ShaderProgram* prog)
{
	PipelineDesc desc;
	desc.program = prog;
	return m_device->newPipeline(desc);
}

bool TriangleApp::init()
{
	struct CustomVertex
	{
		Vector3 position;
		Vector3 color;
	};

	InputElement elements[] = 
	{
		{ SEMANTIC_POSITION},
		{ SEMANTIC_COLOR},
	};

	// 数据,注意旋转方向，否则会不显示的
	float vertex_data[] = 
	{
		-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f
	};

	short index_data[] = { 0, 1, 2 };

	m_vb = m_device->newVertexBuffer(sizeof(CustomVertex), 3, vertex_data);
	m_ib = m_device->newIndexBuffer(INDEX16, 3, index_data);
	m_layout = m_device->newInputLayout(elements, 2);

	m_program = loadProgram("./assets/color.hlsl.vs", "./assets/color.hlsl.ps");
	m_pipeline = newPipeline(m_program);
	m_descriptors = m_device->newDescriptorSet(m_pipeline);
	m_descriptors->setValue("worldMatrix", m_worldMatrix);
	m_descriptors->setValue("viewMatrix", m_viewMatrix);
	m_descriptors->setValue("projMatrix", m_projMatrix);

	m_cmdBuffer->setRenderTarget(m_swapchain);
	m_cmdBuffer->setViewport(0, 0, m_width, m_height);
	m_cmdBuffer->setVertexBuffer(m_vb);
	m_cmdBuffer->setIndexBuffer(m_ib);
	m_cmdBuffer->setPipeline(m_pipeline);
	m_cmdBuffer->setInputLayout(m_layout);
	m_cmdBuffer->setTopology(PT_TRIANGLE_LIST);
	m_cmdBuffer->setDescriptorSet(m_descriptors);

	return true;
}
