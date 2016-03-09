#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class CU_VK_API VKGraphics : public Graphics
{
	CU_NONCOPYABLE(VKGraphics);
public:
	static VKGraphics& instance()
	{
		static VKGraphics inst;
		return inst;
	}

	VKGraphics();
	~VKGraphics();

	VkInstance handle() { return m_handle;  }
	VkPhysicalDevice gpu() { return m_gpu; }

private:
	VkInstance		m_handle;
	VkPhysicalDevice m_gpu;
};

CU_NS_END
