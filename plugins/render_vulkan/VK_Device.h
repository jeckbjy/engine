#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class CU_VK_API VKDevice : public Device
{
	CU_NONCOPYABLE(VKDevice);
public:
	VKDevice();
	virtual ~VKDevice();

private:
	VkDevice m_handle;
};

CU_NS_END
