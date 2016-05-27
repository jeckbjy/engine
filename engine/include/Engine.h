#pragma once
#include "Object.h"
#include "Renderer.h"
#include "Singleton.h"

CU_NS_BEGIN

// 总管理器
class CU_API Engine : public Object
{
	DECLARE_RTTI(Engine, Object, "ENGI");
public:
	Engine();
	virtual ~Engine();

	bool init();
	void update();	// 执行一帧

	Graphics*	getGraphics(){ return m_graphics; }
	void		setGraphics(Graphics* graphics) { m_graphics = graphics; }

protected:
	uint64	m_frame;
	uint64	m_timestamp;
	Renderer* m_renderer;
	Graphics* m_graphics;	// 当前使用graphics
};

extern CU_API Engine gEngine;

CU_NS_END