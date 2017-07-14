#include "VK_Fence.h"
#include "VK_Device.h"

CUTE_NS_BEGIN

VK_Fence* VK_Fence::create(VK_Device* device)
{
	VK_Fence* fence = new VK_Fence();
	if (fence == NULL)
		return NULL;

	if (!fence->init(device))
	{
		delete fence;
		return NULL;
	}

	return fence;
}

VK_Fence::VK_Fence()
	: m_device(NULL)
	, m_fence(VK_NULL_HANDLE)
{
}

VK_Fence::~VK_Fence()
{
	term();
}

bool VK_Fence::init(VK_Device* device)
{
	m_device = device;
	m_device->incRef();

	VkDevice native = device->getDevice();

	VkFenceCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;

	VkResult result = vkCreateFence(native, &info, NULL, &m_fence);
	return result != VK_SUCCESS;
}

void VK_Fence::term()
{
	VkDevice native = m_device->getDevice();
	vkDestroyFence(native, m_fence, NULL);

	m_fence = VK_NULL_HANDLE;
	m_device->decRef();
	m_device = NULL;
}

bool VK_Fence::isSignaled() const
{
	VkDevice device = m_device->getDevice();

	VkResult ret = vkGetFenceStatus(device, m_fence);
	if (ret == VK_SUCCESS)
	{
		vkResetFences(device, 1, &m_fence);
		return true;
	}
	else
	{
		return false;
	}
}

bool VK_Fence::wait(uint32 msec)
{
	const uint64 NANO_RESOLUTION = 1000 * 1000;

	VkDevice device = m_device->getDevice();
	VkResult result = vkWaitForFences(device, 1, &m_fence, VK_TRUE, msec * NANO_RESOLUTION);
	vkResetFences(device, 1, &m_fence);
	return result == VK_SUCCESS;
}

CUTE_NS_END
