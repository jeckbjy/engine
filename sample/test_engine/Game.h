#pragma once
#include "Application.h"
using namespace cute;

class GameApp : public Application
{
public:
	bool init();
};

//#include "Window.h"
////#include "Root.h"
//#include "Transform.h"
//#include "Camera.h"
//#include "ResourceManager.h"
//#include "Model.h"
//using namespace cute;
//
//class App
//{
//public:
//	App(const char* vs = NULL, const char* ps = NULL);
//	~App();
//
//	void run();
//	virtual bool init();
//	virtual void draw();
//	virtual void onMouseEvent(){}
//	virtual void onKeyEvent(){}
//
//protected:
//	Program* loadProgram(const char* vs_path, const char* ps_path);
//
//protected:
//	const char*		m_vs;
//	const char*		m_ps;
//	Window			m_window;
//	Root*			m_root;
//	RenderTarget*	m_target;
//	Program*		m_prog;
//	Pipeline*		m_pipeline;
//	CommandList*	m_command;
//	DescriptorSet*	m_descriptor;
//	Transform		m_world;
//};
//
//class TriangleApp : public App
//{
//public:
//	TriangleApp() :App("assets/simple.vs", "assets/simple.ps"){}
//	bool init();
//
//private:
//	VertexBuffer*	m_vb;
//	IndexBuffer*	m_ib;
//	VertexLayout*	m_layout;
//};
//
//class ProjectionApp : public App
//{
//public:
//	ProjectionApp() :App("assets/shader1.vs", "assets/shader1.ps"){}
//	bool init();
//
//private:
//	VertexBuffer*	m_vb;
//	IndexBuffer*	m_ib;
//	VertexLayout*	m_layout;
//};
//
//class TextureApp : public App
//{
//public:
//	TextureApp() :App("assets/stex.vs", "assets/stex.ps"){}
//	bool init();
//
//private:
//	VertexBuffer*	m_vb;
//	IndexBuffer*	m_ib;
//	VertexLayout*	m_layout;
//	Texture*		m_tex;
//};
//
//class MeshApp : public App
//{
//public:
//	MeshApp() :App("assets/standard.vs", "assets/standard.ps"){}
//	bool init();
//	void draw();
//
//private:
//	Material*	m_material;
//	Texture*	m_tex;
//	//Model		m_model;
//};
//
//class SkinMeshApp : public App
//{
//public:
//	SkinMeshApp() :App("assets/skined.vs", "assets/skined.ps"){}
//private:
//};