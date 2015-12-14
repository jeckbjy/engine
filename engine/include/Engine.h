#pragma once
#include "Object.h"

CU_NS_BEGIN

class CU_API Engine : public Object
{
	DECLARE_RTTI(Engine, Object, "ENGI");
public:
	Engine();
	virtual ~Engine();

	bool init();
	void quit();
	void run();
	void update();	// Ö´ÐÐÒ»Ö¡

private:
	bool m_quit;
};

CU_NS_END