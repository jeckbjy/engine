#pragma once
#include "Object.h"
#include "Renderer.h"
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

protected:
	uint64	m_frame;
	uint64	m_timestamp;
	Renderer* m_renderer;
};

extern Engine gEngine;

CU_NS_END