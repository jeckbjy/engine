#pragma once
#include "Window.h"

CU_NS_BEGIN

// �����,������һ��������,���������Ӵ���
class CU_API Application
{
public:
	Application();
	~Application();

	virtual bool init(){ return true; }
	virtual void release(){}
	virtual void update(){}
	void run();

	void pumpMsg();

protected:
	bool	m_quit;
	Window* m_main;	// ������
};

CU_NS_END