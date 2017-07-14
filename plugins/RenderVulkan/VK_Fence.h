#pragma once
#include "VK_API.h"

CUTE_NS_BEGIN

class VK_Device;
class CUTE_VK_API VK_Fence : public IFence
{
public:
	static VK_Fence* create(VK_Device* device);

	VK_Fence();
	~VK_Fence();

	bool init(VK_Device* device);
	void term();

	bool isSignaled() const;
	bool wait(uint32 msec);

private:
	VK_Device*	m_device;
	VkFence		m_fence;
};

CUTE_NS_END
