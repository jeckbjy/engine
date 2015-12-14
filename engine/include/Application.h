#pragma once
#include "Window.h"

CU_NS_BEGIN

// 主入口,必须有一个主窗体,其他都是子窗体
class CU_API Application
{
public:
	Application();
	~Application();

	virtual void init(){}
	virtual void release(){}
	int run();

protected:
	//Window* m_main;	// 主窗体
};

CU_NS_END