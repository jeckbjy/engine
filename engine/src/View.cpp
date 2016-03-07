#include "View.h"
#include "Renderer.h"
#include "RenderPath.h"
#include "SceneManager.h"

CU_NS_BEGIN

View::View(Renderer* renderer, Camera* camera)
: m_render(renderer)
, m_camera(camera)
{

}

View::~View()
{

}

void View::addBatch(Batch* batch)
{
	if (batch->isTransparent())
		m_transparents.push(batch);
	else
		m_opaques.push(batch);
}

void View::createGBuffer()
{
	m_targets.resize(m_path->targets.size());
	Vector2 size;
	for (size_t i = 0; i < m_path->targets.size(); ++i)
	{
		const RenderTargetInfo& info = m_path->targets[i];
		switch (info.mode)
		{
		case SIZE_MODE_ABSOLUTE:
			size = info.size;
			break;
		case SIZE_MODE_DIVISOR:
			size = m_viewSize / info.size;
			break;
		case SIZE_MODE_MULTIPLIER:
			size = m_viewSize * info.size;
		}

		size_t width  = (size_t)(size.x + 0.5f);
		size_t height = (size_t)(size.y + 0.5f);
		//unsigned persistent = info.persistent ? hash(info.name) : 0;
		uint persistent = 0;
		//m_targets[i] = m_render->getGBuffer(width, height, info.format, info.cubemap, info.filtered, info.sRGB, persistent);
	}
}

void View::setRenderTargets(const RenderCommand& command)
{
	size_t index = 0;

}

void View::setTextures(const RenderCommand& command)
{

}

void View::reset()
{
	m_opaques.clear();
	m_transparents.clear();
}

void View::update()
{
	Octree* octree = gSceneMgr().getOctree();
	// get visiable drawable
	Vector<Drawable*> drawables;
	FrustumOctreeQuery query;
	octree->query(query);
	
	// get batches and sort batches
	for (auto itor = drawables.begin(); itor != drawables.end(); ++itor)
	{
		(*itor)->draw(this);
	}

	m_opaques.sortBackToFront();
	m_transparents.sortFrontToBack();

	// process lights??
	// prepare targets
	// do render
}

void View::render()
{
	// do clear
	for (size_t i = 0; i < m_path->commands.size(); ++i)
	{
		RenderCommand& command = m_path->commands[i];
		switch (command.type)
		{
		case CMD_SCENE_PASS:
		{
			// 根据不同queue执行
			//BatchQueue& queue = command
			break;
		}
		case CMD_FORWARD_LIGHTS:
		{
			break;
		}
		}
	}
}

CU_NS_END
