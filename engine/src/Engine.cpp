#include "Engine.h"
#include "Util.h"
#include "SceneManager.h"

CU_NS_BEGIN

Engine gEngine;

Engine::Engine()
{
	m_renderer = new Renderer();
}

Engine::~Engine()
{
	delete m_renderer;
}

bool Engine::init()
{
	return true;
}

void Engine::update()
{
	// 更新时间戳
	uint64 curr_time = Util::time();
	// limit fps if needed
	if (m_frame > 0)
	{
		uint64 next_time = m_timestamp + m_frame;
		if (next_time > curr_time)
		{
			Util::msleep(next_time - curr_time);
			curr_time = Util::time();
		}
	}

	m_timestamp = curr_time;
	// update scene
	SceneManager::Instance().update();
	// update query
	// update render
	m_renderer->update();
}

void Engine::loadPlugin(const String& path, bool install)
{
	m_plugins.load(path, install);
}

CU_NS_END
