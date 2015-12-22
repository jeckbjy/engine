#include "Game.h"
//#include <iostream>
//#include <fstream>
//#include <streambuf>
//using namespace std;
//
////#include "glm/glm.hpp"
////#include <glm/gtc/matrix_transform.hpp>
//
//App::App(const char* vs, const char* ps)
//	:m_vs(vs), m_ps(ps), m_target(NULL), m_pipeline(NULL), m_descriptor(NULL)
//{
//
//}
//
//App::~App()
//{
//	CU_SAFE_RELEASE(m_target);
//	CU_SAFE_RELEASE(m_pipeline);
//}
//
//Program* App::loadProgram(const char* vs_path, const char* ps_path)
//{
//	if (vs_path == NULL)
//		return NULL;
//	string vs_code;
//	string ps_code;
//	ifstream stream(vs_path, std::ios::in);
//	if (!stream.is_open())
//		return NULL;
//	vs_code.assign(istreambuf_iterator<char>(stream), istreambuf_iterator<char>());
//	stream.close();
//	stream.open(ps_path, std::ios::in);
//	if (!stream.is_open())
//		return NULL;
//	ps_code.assign(istreambuf_iterator<char>(stream), istreambuf_iterator<char>());
//	Program* prog = m_root->createProgram();
//	if (!prog->attach(ST_VERTEX, vs_code))
//	{
//		prog->release();
//		return NULL;
//	}
//	if (!prog->attach(ST_PIXEL, ps_code))
//	{
//		prog->release();
//		return NULL;
//	}
//	if (!prog->link())
//	{
//		prog->release();
//		return NULL;
//	}
//	return prog;
//}
//
//void App::run()
//{
//	if (!init())
//		return;
//	m_window.show();
//	do 
//	{
//		m_root->update();
//		m_target->clear();
//		m_command->setViewport(0, 0, m_window.width(), m_window.height());
//		draw();
//		m_root->execute();
//		m_target->present();
//		// 执行时间
//		if (Window::pollEvents())
//			break;
//	} while (!m_window.isClosed());
//}
//
//bool App::init()
//{
//	const char* plugins[] = { "plugin_ogl", "plugin_assimp" };
//
//	m_root = Root::InstancePtr();
//	// 加载plugin
//	for (int i = 0; i < CU_ARRAY_SIZE(plugins); ++i)
//	{
//		m_root->loadPlugin(plugins[i]);
//	}
//
//	m_window.create("test", 1024, 768, WF_RESIZABLE);
//	m_target = m_root->createRenderTarget(&m_window);
//	m_command = m_root->getCommandList();
//	if (m_vs)
//	{
//		m_prog = loadProgram(m_vs, m_ps);
//		m_pipeline = m_root->createPipeline(m_prog);
//		m_descriptor = m_root->createDescriptorSet(m_prog);
//		m_command->setPipeline(m_pipeline);
//	}
//	return true;
//}
//
//void App::draw()
//{
//	m_root->draw(m_command);
//}
//
//bool TriangleApp::init()
//{
//	App::init();
//	struct CustomVertex
//	{
//		Vector3 position;
//		Vector3 color;
//	};
//
//	VertexElement elements[] = {
//		{ VU_POSITION, PF_FLOAT3 },
//		{ VU_COLOR, PF_FLOAT3 },
//	};
//	// 准备数据
//	float vertex_data[] = {
//		-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//		1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
//		0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
//	};
//	short index_data[] = { 0, 1, 2 };
//
//	//
//	m_vb = m_root->createVertexBuffer(sizeof(CustomVertex), 3, vertex_data);
//	m_ib = m_root->createIndexBuffer(INDEX16, 3, index_data);
//	m_layout = m_root->createVertexLayout(VertexDeclaration(elements, 2));
//	m_layout->setBuffer(m_vb);
//	m_command->setVertexLayout(m_layout);
//	m_command->setIndexBuffer(m_ib);
//	m_command->drawIndexed(0);
//	return true;
//}
//
//bool ProjectionApp::init()
//{
//	App::init();
//
//	VertexElement elements[] = {
//		{ VU_POSITION }
//	};
//	// 准备数据
//	float vertex_data[] = {
//		-1.0f, -1.0f, 0.5773f,
//		0.0f, -1.0f, -1.15475f,
//		1.0f, -1.0f, 0.5773f,
//		0.0f, 1.0f, 0.0f
//	};
//	short index_data[] = { 
//		0, 3, 1,
//		1, 3, 2,
//		2, 3, 0,
//		0, 1, 2
//	};
//
//	//
//	m_vb = m_root->createVertexBuffer(sizeof(Vector3), 4, vertex_data);
//	m_ib = m_root->createIndexBuffer(INDEX16, 12, index_data);
//	m_layout = m_root->createVertexLayout(VertexDeclaration(elements, CU_ARRAY_SIZE(elements)));
//	m_layout->setBuffer(m_vb);
//
//	//
//	Matrix4 proj = Matrix4::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
//	Matrix4 view = Matrix4::lookAt(Vector3(0, 0, 3), Vector3::Zero, Vector3::UnitY);
//	Matrix4 model = Matrix4(1.0f);
//	Matrix4 mvp = model * view * proj;
//	GpuBuffer* buffer = m_root->createUniformBuffer(0);
//	buffer->writeBuffer(mvp.data(), sizeof(mvp));
//	m_descriptor->bind("gWorld", buffer);
//
//	m_command->setVertexLayout(m_layout);
//	m_command->setIndexBuffer(m_ib);
//	m_command->setDescriptorSet(m_descriptor);
//	m_command->drawIndexed(0);
//
//	return true;
//}
//
//bool TextureApp::init()
//{
//	App::init();
//
//	struct CustomVertexTex
//	{
//		float position[3];
//		float uv[2];
//	};
//
//	VertexElement elements[] = {
//		{ VU_POSITION, PF_FLOAT3 },
//		{ VU_TEXCOORD0, PF_FLOAT2 },
//	};
//
//	float tex_vertex_data[] = {
//		-1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
//		1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
//		1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
//		-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
//	};
//
//	short tex_index_data[] = { 0, 1, 2, 2, 3, 0 };
//
//	m_vb = m_root->createVertexBuffer(sizeof(CustomVertexTex), 4, tex_vertex_data);
//	m_ib = m_root->createIndexBuffer(INDEX16, 6, tex_index_data);
//	m_layout = m_root->createVertexLayout(VertexDeclaration(elements, 2));
//	m_layout->setBuffer(m_vb);
//	m_tex = ResourceManager::load<Texture>("assets/uvtemplate.dds");
//	if (!m_tex)
//		return false;
//
//	Matrix4 proj = Matrix4::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
//	Matrix4 view = Matrix4::lookAt(Vector3(0, 0, 3), Vector3::Zero, Vector3::UnitY);
//	Matrix4 model(1.0);
//	Matrix4 mvp = model * view * proj;
//	UniformBuffer* buffer = m_root->createUniformBuffer(0);
//	buffer->writeBuffer(mvp.data(), sizeof(mvp));
//	m_descriptor->bind("MVP", buffer);
//	m_descriptor->bind("tex", m_tex);
//
//	m_command->setVertexLayout(m_layout);
//	m_command->setIndexBuffer(m_ib);
//	m_command->setDescriptorSet(m_descriptor);
//	m_command->drawIndexed(0);
//	return true;
//}
//
//bool MeshApp::init()
//{
//	App::init();
//	Pass* pass = m_pipeline;
//	Technique* tech = new Technique();
//	tech->addPass(pass);
//	Shader* shader = new Shader();
//	shader->addTechnique(tech);
//	m_material = new Material(shader);
//	
//	//m_tex = AssetManager::Instance().load<Texture>("uvmap.dds");
//	m_tex = ResourceManager::Instance().load<Texture>("assets/Jellyfish.bmp");
//
//	Mesh* mesh = ResourceManager::load<Mesh>("assets/suzanne.obj");
//	//Mesh* mesh = AssetManager::load<Mesh>("assets/boblampclean.md5mesh");
//	//m_model.setMesh(mesh);
//	//m_model.addMeterial(m_material);
//
//	Matrix4 proj = Matrix4::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
//	Matrix4 view = Matrix4::lookAt(Vector3(0, 0, 3), Vector3::Zero, Vector3::UnitY);
//	Matrix4 model = Matrix4(1.0f);
//	Matrix4 mvp = model * view * proj;
//	Matrix4 mv = model * view;
//	Vector3 light(4, 4, 4);
//
//	m_material->set("MVP", mvp);
//	m_material->set("V", view);
//	m_material->set("M", model);
//	m_material->set("MV", mv);
//	m_material->set("LightPosition_worldspace", light);
//	m_material->set("myTextureSampler", m_tex);
//
//	return true;
//}
//
//void MeshApp::draw()
//{
//	//m_model.draw();
//}