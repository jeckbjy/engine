#include "VK_Buffer.h"
#include "VK_Mapping.h"
#include "VK_Device.h"

CUTE_NS_BEGIN

VK_Buffer::VK_Buffer()
	: m_device(NULL)
	, m_buffer(VK_NULL_HANDLE)
	, m_memory(VK_NULL_HANDLE)
	, m_state(RESOURCE_STATE_UNKNOWN)
{
}

VK_Buffer::~VK_Buffer()
{
	term();
}

bool VK_Buffer::init(VK_Device* device, const BufferDesc& desc)
{
	VkDevice native;
	VkResult ret;
	if (device == NULL)
		return false;

	m_device = device;
	native   = device->getDevice();

	// create buffer
	VkBufferCreateInfo info;
	info.sType					= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	info.pNext					= NULL;
	info.flags					= 0;
	info.pQueueFamilyIndices	= NULL;
	info.queueFamilyIndexCount	= 0;
	info.sharingMode			= VK_SHARING_MODE_EXCLUSIVE;
	info.size					= desc.size;
	info.usage					= VK_Mapping::getBufferUsage(desc.usage);

	ret = vkCreateBuffer(native, &info, NULL, &m_buffer);
	if (ret != VK_SUCCESS)
		return false;

	// alloc memory
	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(native, m_buffer, &requirements);

	VkMemoryPropertyFlags flags = VK_Mapping::getMemoryPropertyFlags(desc.heapProperty.cpuPageProperty, true);

	if (!device->alloc(m_memory, requirements, flags))
		return false;

	// bind buffer
	ret = vkBindBufferMemory(native, m_buffer, m_memory, 0);
	if (ret != VK_SUCCESS)
		return false;

	m_state = desc.state;
	return true;
}

void VK_Buffer::term()
{
	VkDevice native;

	if (m_device == NULL)
		return;

	native = m_device->getDevice();
	if (m_memory != VK_NULL_HANDLE)
	{
		vkFreeMemory(native, m_memory, NULL);
		m_memory = VK_NULL_HANDLE;
	}

	if (m_buffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(native, m_buffer, NULL);
		m_buffer = VK_NULL_HANDLE;
	}

	// ÊÍ·Ådevice
}

void* VK_Buffer::map()
{
	VkDevice native;
	VkResult ret;
	void*	 data;

	native = m_device->getDevice();
	ret = vkMapMemory(native, m_memory, 0, m_size, 0, &data);
	if (ret != VK_SUCCESS)
		return NULL;

	VkMappedMemoryRange range = {};
	range.sType		= VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	range.pNext		= NULL;
	range.memory	= m_memory;
	range.offset	= 0;
	range.size		= m_size;

	ret = vkInvalidateMappedMemoryRanges(native, 1, &range);
	if (ret != VK_SUCCESS)
		return NULL;

	return data;
}

void VK_Buffer::unmap()
{
	VkDevice native = m_device->getDevice();
	    
	VkMappedMemoryRange range = {};
    range.sType     = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    range.pNext     = NULL;
    range.memory    = m_memory;
    range.offset    = 0;
    range.size      = m_size;
    vkFlushMappedMemoryRanges(native, 1, &range);

    vkUnmapMemory(native, m_memory);
}

//VK_Buffer::VK_Buffer(VK_Device* device, const BufferDesc& desc)
//	: GpuBuffer(desc)
//	, m_device(device)
//	, m_buffer(VK_NULL_HANDLE)
//	, m_memory(VK_NULL_HANDLE)
//{
//	m_device = device;
//	VkBufferCreateInfo info;
//	memset(&info, 0, sizeof(info));
//	info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//	info.usage = VK_Mapping::getBufferUsage(desc.usage);
//	info.size = m_bytes;
//	VK_CHECK(vkCreateBuffer(m_device->native(), &info, NULL, &m_buffer), "vkCreateBuffer fail!");
//
//	VkMemoryRequirements reqs;
//	vkGetBufferMemoryRequirements(m_device->native(), m_buffer, &reqs);
//	// alloc
//	m_device->allocMemory(m_memory, m_bytes, reqs.memoryTypeBits);
//
//	// bind
//	vkBindBufferMemory(m_device->native(), m_buffer, m_memory, 0);
//}
//
//VK_Buffer::~VK_Buffer()
//{
//	if (m_memory != VK_NULL_HANDLE)
//		vkFreeMemory(m_device->native(), m_memory, NULL);
//
//	if (m_buffer != VK_NULL_HANDLE)
//		vkDestroyBuffer(m_device->native(), m_buffer, NULL);
//}
//
//void* VK_Buffer::map(size_t offset, size_t len, MAP_FLAG flags)
//{
//	// flags is reserved for future use, and must be zero.
//	CU_UNUSED(flags);
//	void* data;
//	VK_CHECK(vkMapMemory(m_device->native(), m_memory, offset, len, 0, &data),  "vkMapMemory fail!");
//	return data;
//}
//
//void VK_Buffer::unmap()
//{
//	vkUnmapMemory(m_device->native(), m_memory);
//}

CUTE_NS_END
