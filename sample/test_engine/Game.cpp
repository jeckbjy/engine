#include "Game.h"
#include "AssetCache.h"
#include "Model.h"

BaseApp::BaseApp()
	: m_width(1024)
	, m_height(768)
{
}

BaseApp::~BaseApp()
{

}

void BaseApp::draw()
{
	m_cmdQueue->submit(m_cmdBuffer, NULL);
}

void BaseApp::run()
{
	m_device = gGraphics.getDevice();
	if (!m_device)
		return;

	WINDOW_DESC desc;
	desc.width = m_width;
	desc.height = m_height;
	desc.title = "Game";

	m_window = new Window(desc);

	m_cmdBuffer = m_device->newCommandBuffer();
	m_cmdQueue = m_device->newCommandQueue();
	m_swapchain = m_device->newSwapChain(m_window);
	m_cmdBuffer->setViewport(0, 0, m_window->getWidth(), m_window->getHeight());
	m_cmdBuffer->setRenderTarget(m_swapchain);

	if (!init())
		return;

	m_window->show();
	do 
	{
		draw();
		// 执行时间
		if (Window::pollEvents())
			break;
	} while (!m_window->isClosed());
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

	InputElement elements[] = {
		{ SEMANTIC_POSITION},
		{ SEMANTIC_COLOR},
	};

	// 数据
	float vertex_data[] = {
		-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	short index_data[] = { 0, 1, 2 };

	m_vb = m_device->newVertexBuffer(sizeof(CustomVertex), 3, vertex_data);
	m_ib = m_device->newIndexBuffer(INDEX16, 3, index_data);

	m_program = loadProgram("triangle.vs", "triangle.ps");
	m_pipeline = newPipeline(m_program);
	m_layout = m_device->newInputLayout(elements, 2);

	m_cmdBuffer->setRenderTarget(m_swapchain);
	m_cmdBuffer->setViewport(0, 0, m_width, m_height);
	m_cmdBuffer->setVertexBuffer(m_vb);
	m_cmdBuffer->setIndexBuffer(m_ib);
	m_cmdBuffer->setPipeline(m_pipeline);
	m_cmdBuffer->setInputLayout(m_layout);

	m_cmdBuffer->clear();
	m_cmdBuffer->drawIndexed(m_ib->bytes());

	return true;
}
