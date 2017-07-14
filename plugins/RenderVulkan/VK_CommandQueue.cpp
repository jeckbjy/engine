#include "VK_CommandQueue.h"
#include "VK_CommandList.h"
#include "VK_Device.h"

CUTE_NS_BEGIN

bool VK_CommandQueue::create(VK_Device* device, uint32 familyIndex, uint32 queueIndex, uint32 maxSubmitCount, VK_CommandQueue** queue)
{
	VK_CommandQueue* instance = new VK_CommandQueue();
	if (instance == NULL)
		return false;

	if (!instance->init(device, familyIndex, queueIndex, maxSubmitCount))
	{
		delete instance;
		return false;
	}

	*queue = instance;
	return true;
}

VK_CommandQueue::VK_CommandQueue()
	: m_device(NULL)
	, m_queue(VK_NULL_HANDLE)
	, m_submitLists(NULL)
	, m_submitCount(0)
	, m_submitIndex(0)
	, m_familyIndex(0)
	, m_currIndex(0)
	, m_prevIndex(0)
{
	for (int i = 0; i < MAX_BUFFER_COUNT; ++i)
	{
		m_waitSemp[i]	= VK_NULL_HANDLE;
		m_signalSemp[i] = VK_NULL_HANDLE;
		m_fence[i]		= VK_NULL_HANDLE;
	}
}

VK_CommandQueue::~VK_CommandQueue()
{
	term();
}

bool VK_CommandQueue::init(VK_Device* device, uint32 familyIndex, uint32 queueIndex, uint32 maxSubmitCount)
{
	if (device == NULL)
		return false;

	VkDevice native;
	VkResult ret;

	m_device = device;
	m_device->incRef();
	native = device->getDevice();

	// create semaphore and fence
	VkSemaphoreCreateInfo sempInfo = {};
	sempInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	sempInfo.pNext = NULL;
	sempInfo.flags = 0;
	
	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.pNext = nullptr;
	fenceInfo.flags = 0;

	for (int i = 0; i < MAX_BUFFER_COUNT; ++i)
	{
		// create semaphore
		ret = vkCreateSemaphore(native, &sempInfo, NULL, &m_signalSemp[i]);
		if (ret != VK_SUCCESS)
			return false;

		ret = vkCreateSemaphore(native, &sempInfo, NULL, &m_waitSemp[i]);
		if (ret != VK_SUCCESS)
			return false;

		// create fence
		ret = vkCreateFence(native, &fenceInfo, NULL, &m_fence[i]);
		if (ret != VK_SUCCESS)
			return false;

		ret = vkResetFences(native, 1, &m_fence[i]);
		if (ret != VK_SUCCESS)
			return false;
	}
	
	// get queue
	vkGetDeviceQueue(native, familyIndex, queueIndex, &m_queue);

	m_submitLists = new VkCommandBuffer[maxSubmitCount];
	if (m_submitLists == NULL)
		return false;

	for (int i = 0; i < maxSubmitCount; ++i)
		m_submitLists[i] = VK_NULL_HANDLE;

	m_submitCount = maxSubmitCount;
	m_familyIndex = familyIndex;
	m_submitIndex = 0;
	m_currIndex = 0;
	m_prevIndex = 0;

	return true;
}

void VK_CommandQueue::term()
{
	VkDevice device;
	if (m_device == NULL)
		return;

	device = m_device->getDevice();

	if (m_queue != VK_NULL_HANDLE)
	{
		vkQueueWaitIdle(m_queue);
		m_queue = VK_NULL_HANDLE;
	}

	for (int i = 0; i < MAX_BUFFER_COUNT; ++i)
	{
		if (m_signalSemp[i] != VK_NULL_HANDLE)
		{
			vkDestroySemaphore(device, m_signalSemp[i], NULL);
			m_signalSemp[i] = VK_NULL_HANDLE;
		}

		if (m_waitSemp[i] != VK_NULL_HANDLE)
		{
			vkDestroySemaphore(device, m_waitSemp[i], NULL);
			m_waitSemp[i] = VK_NULL_HANDLE;
		}

		if (m_fence[i] != VK_NULL_HANDLE)
		{
			vkDestroyFence(device, m_fence[i], NULL);
			m_fence[i] = VK_NULL_HANDLE;
		}
	}

	if (m_submitLists != NULL)
	{
		delete[] m_submitLists;
		m_submitLists = NULL;
	}

	m_submitIndex = 0;
	m_submitCount = 0;
	m_familyIndex = 0;
	m_queue = VK_NULL_HANDLE;
	m_device->decRef();
	m_device = NULL;
}

bool VK_CommandQueue::resetSync()
{
	VkDevice device = m_device->getDevice();

	// destroy
	for (int i = 0; i < MAX_BUFFER_COUNT; ++i)
	{
		if (m_signalSemp[i] != VK_NULL_HANDLE)
		{
			vkDestroySemaphore(device, m_signalSemp[i], NULL);
			m_signalSemp[i] = VK_NULL_HANDLE;
		}

		if (m_waitSemp[i] != VK_NULL_HANDLE)
		{
			vkDestroySemaphore(device, m_waitSemp[i], NULL);
			m_waitSemp[i] = VK_NULL_HANDLE;
		}

		if (m_fence[i] != VK_NULL_HANDLE)
		{
			vkDestroyFence(device, m_fence[i], NULL);
			m_fence[i] = VK_NULL_HANDLE;
		}
	}

	// create
	VkSemaphoreCreateInfo sempInfo = {};
	sempInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	sempInfo.pNext = NULL;
	sempInfo.flags = 0;

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.pNext = NULL;
	fenceInfo.flags = 0;

	VkResult ret;
	for (int i = 0; i < MAX_BUFFER_COUNT; ++i)
	{
		// create semaphore
		ret = vkCreateSemaphore(device, &sempInfo, NULL, &m_signalSemp[i]);
		if (ret != VK_SUCCESS)
			return false;

		ret = vkCreateSemaphore(device, &sempInfo, NULL, &m_waitSemp[i]);
		if (ret != VK_SUCCESS)
			return false;

		// create fence
		ret = vkCreateFence(device, &fenceInfo, NULL, &m_fence[i]);
		if (ret != VK_SUCCESS)
			return false;

		ret = vkResetFences(device, 1, &m_fence[i]);
		if (ret != VK_SUCCESS)
			return false;
	}

	m_prevIndex = 0;
	m_currIndex = 0;

	return true;
}

bool VK_CommandQueue::submit(ICommandList* cmds)
{
	Mutex::ScopedLock guard(m_mutex);

	if (m_submitIndex + 1 >= m_submitCount)
		return false;

	VK_CommandList* wrapList = cmds->cast<VK_CommandList>();
	VkCommandBuffer native = wrapList->getNative();
	m_submitLists[m_submitIndex] = native;
	m_submitIndex++;
	return true;
}

void VK_CommandQueue::execute(IFence* fence)
{
	VkPipelineStageFlags stageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

	VkFence nativeFence = VK_NULL_HANDLE;
	VkSubmitInfo info = {};

	if (fence != NULL)
	{
		info.sType					= VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.pNext					= NULL;
		info.pCommandBuffers		= m_submitLists;
		info.commandBufferCount		= m_submitIndex;
		info.waitSemaphoreCount		= 1;
		info.pWaitSemaphores		= &m_waitSemp[m_currIndex];
		info.pWaitDstStageMask		= &stageMask;
		info.signalSemaphoreCount	= 0;
		info.pSignalSemaphores		= NULL;
	}
	else
	{
		info.sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        info.pNext                  = NULL;
        info.pCommandBuffers        = m_submitLists;
        info.commandBufferCount     = m_submitIndex;
        info.waitSemaphoreCount     = 0;
        info.pWaitSemaphores        = NULL;
        info.pWaitDstStageMask      = &stageMask;
        info.signalSemaphoreCount   = 0;
        info.pSignalSemaphores      = NULL;
	}

	VkResult ret = vkQueueSubmit(m_queue, 1, &info, nativeFence);
	assert(ret == VK_SUCCESS);
	CUTE_UNUSED(ret);

	m_submitIndex = 0;
	m_prevIndex = m_currIndex;
	m_currIndex = (m_currIndex + 1) % MAX_BUFFER_COUNT;
}

void VK_CommandQueue::waitIdle()
{
	VkResult ret = vkQueueWaitIdle(m_queue);
	assert(ret == VK_SUCCESS);
	CUTE_UNUSED(ret);

	m_prevIndex = m_currIndex;
	m_currIndex = 0;
}

VkQueue VK_CommandQueue::getQueue() const
{
	return m_queue;
}

VkFence VK_CommandQueue::getFence(size_t index) const
{
	return m_fence[m_currIndex];
}

VkSemaphore VK_CommandQueue::getWaitSemapthore() const
{
	return m_waitSemp[m_currIndex];
}

VkSemaphore VK_CommandQueue::getSignalSemapthore() const
{
	return m_signalSemp[m_currIndex];
}

uint32 VK_CommandQueue::getFamilyIndex() const
{
	return m_familyIndex;
}

CUTE_NS_END
