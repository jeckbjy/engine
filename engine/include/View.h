#pragma once
#include "Graphics.h"
#include "Batch.h"
#include "Vector2.h"
#include "RenderPath.h"

CU_NS_BEGIN

// 渲染窗体
class CU_API View
{
public:
	View(Renderer* renderer, Camera* camera);
	~View();

	void reset();
	void update();	// 更新数据
	void render();	// 全部渲染

	void addBatch(Batch* batch);
	void createGBuffer();
	void setRenderTargets(const RenderCommand& command);
	void setTextures(const RenderCommand& command);

	Camera* getCamera() { return m_camera; }

protected:
	Renderer*	m_render;
	Camera*		m_camera;
	RenderPath*	m_path;
	BatchQueue	m_opaques;
	BatchQueue	m_transparents;
	Vector2		m_viewSize;
};

CU_NS_END
