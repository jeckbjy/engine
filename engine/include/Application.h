#pragma once
#include "Window.h"

CU_NS_BEGIN

// �����,������һ��������,���������Ӵ���
class CU_API Application
{
public:
	Application();
	~Application();

	virtual void init(){}
	virtual void release(){}
	int run();

protected:
	//Window* m_main;	// ������
};

CU_NS_END