#pragma once
#include "VK_API.h"
#include "Cute/Mutex.h"

CUTE_NS_BEGIN

class VK_Device;
class CUTE_VK_API VK_CommandQueue : public ICommandQueue
{
public:
	enum
	{
		MAX_BUFFER_COUNT = 2
	};

	static bool create(VK_Device* device, uint32 familyIndex, uint32 queueIndex, uint32 maxSubmitCount, VK_CommandQueue** queue);

	VK_CommandQueue();
	~VK_CommandQueue();

	bool init(VK_Device* device, uint32 familyIndex, uint32 queueIndex, uint32 maxSubmitCount);
	void term();
	bool resetSync();

	bool submit(ICommandList* cmds);
	void execute(IFence* fence);
	void waitIdle();

	VkQueue		getQueue() const;
	VkFence		getFence() const;
	VkSemaphore	getWaitSemapthore() const;
	VkSemaphore getSignalSemapthore() const;
	uint32		getFamilyIndex() const;
	//uint32		getCurrentBufferIndex() const;
	//VkFence		getFence(size_t index) const;
	//VkSemaphore getWaitSemapthore(size_t index) const;
	//VkSemaphore getSignalSemapthore(size_t index) const;

private:
	VK_Device*			m_device;
	VkQueue				m_queue;
	VkCommandBuffer*	m_submitLists;
	uint32				m_submitCount;
	uint32				m_submitIndex;
	uint32				m_familyIndex;
	VkSemaphore			m_signalSemp[MAX_BUFFER_COUNT];
	VkSemaphore			m_waitSemp[MAX_BUFFER_COUNT];
	VkFence				m_fence[MAX_BUFFER_COUNT];
	uint32				m_currIndex;
	uint32				m_prevIndex;
	Mutex				m_mutex;
};

CUTE_NS_END
