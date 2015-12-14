#include "Engine.h"

CU_NS_BEGIN

Engine::Engine()
{

}

Engine::~Engine()
{

}

bool Engine::init()
{
	// ´´½¨´°Ìå£¿£¿
	return true;
}

void Engine::quit()
{
	m_quit = true;
}

void Engine::run()
{
	if (!init())
		return;
	while (!m_quit)
		update();
}

void Engine::update()
{

}

CU_NS_END