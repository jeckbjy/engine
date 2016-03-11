#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class CU_VK_API VK_Graphics : public Graphics
{
	CU_NONCOPYABLE(VK_Graphics);
public:
	VK_Graphics();
	~VK_Graphics();

	void setPhysical(size_t index);
	uint32_t getMemoryType(uint32_t typeBits, VkFlags properties);

	VkInstance& handle() { return m_handle;  }
	VkPhysicalDevice& getPhysicalDevice() { return m_gpu; }

private:
	typedef std::vector<VkPhysicalDevice> PhysicalArray;
	VkInstance		m_handle;
	uint32_t		m_gpuCount;
	PhysicalArray	m_physicals;
	VkPhysicalDevice m_gpu;			// 当前使用的
	size_t			m_queueCount;	// 当前队列
	VkPhysicalDeviceMemoryProperties m_memProps;

};

CU_NS_END
