#pragma once
#include "Graphics.h"

CU_NS_BEGIN

// ��Ⱦ����
class CU_API RenderQueue
{
public:
	RenderQueue();
	~RenderQueue();

	void add();
	void clear();
	void execute();
private:
};

CU_NS_END
