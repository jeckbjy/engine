#pragma once
#include "Object.h"
#include "Renderer.h"
#include "PluginMgr.h"
#include "Singleton.h"

CU_NS_BEGIN

// �ܹ�����
class CU_API Engine : public Object
{
	DECLARE_RTTI(Engine, Object, "ENGI");
public:
	Engine();
	virtual ~Engine();

	bool init();
	void update();	// ִ��һ֡

	void loadPlugin(const String& path, bool install = true);

protected:
	uint64		m_frame;
	uint64		m_timestamp;
	Renderer*	m_renderer;
	PluginMgr	m_plugins;
};

extern CU_API Engine gEngine;

CU_NS_END