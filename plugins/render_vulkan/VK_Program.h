#pragma once
#include "VK_Shader.h"

CU_NS_BEGIN

class CU_VK_API VK_Program : public ShaderProgram
{
public:
	VK_Program(uint32_t id);
	~VK_Program();

	void attach(ShaderStage* shader);
	void link();

	bool isCompute() const { return false; }

private:
};

CU_NS_END