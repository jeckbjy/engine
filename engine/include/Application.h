#pragma once
#include "Window.h"

CU_NS_BEGIN

// �����,������һ��������,���������Ӵ���
class CU_API Application
{
public:
	Application();
	~Application();

	virtual bool setup() { return true; }
	virtual void cleanup(){}
	virtual void update(){}
	void run();

	bool dispatchMsg();

protected:
	bool	m_quit;
	Window* m_main;	// ������
};

CU_NS_END
