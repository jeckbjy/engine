#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class CU_VK_API VKDevice : public Device
{
public:
	VKDevice();
	~VKDevice();

private:
	VkDevice m_handle;
};

CU_NS_END