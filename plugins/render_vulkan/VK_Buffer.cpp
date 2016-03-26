#include "VK_Buffer.h"
#include "VK_Mapping.h"
#include "VK_Device.h"

CU_NS_BEGIN

VK_Buffer::VK_Buffer(VK_Device* device, const BufferDesc& desc)
	: GpuBuffer(desc)
	, m_device(device)
	, m_buffer(VK_NULL_HANDLE)
	, m_memory(VK_NULL_HANDLE)
{
	m_device = device;
	VkBufferCreateInfo info;
	memset(&info, 0, sizeof(info));
	info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	info.usage = VK_Mapping::getBufferUsage(desc.usage);
	info.size = m_bytes;
	VK_CHECK(vkCreateBuffer(m_device->native(), &info, NULL, &m_buffer), "vkCreateBuffer fail!");

	VkMemoryRequirements reqs;
	vkGetBufferMemoryRequirements(m_device->native(), m_buffer, &reqs);
	// alloc
	m_device->allocMemory(m_memory, m_bytes, reqs.memoryTypeBits);

	// bind
	vkBindBufferMemory(m_device->native(), m_buffer, m_memory, 0);
}

VK_Buffer::~VK_Buffer()
{
	if (m_memory != VK_NULL_HANDLE)
		vkFreeMemory(m_device->native(), m_memory, NULL);

	if (m_buffer != VK_NULL_HANDLE)
		vkDestroyBuffer(m_device->native(), m_buffer, NULL);
}

void* VK_Buffer::map(size_t offset, size_t len, MAP_FLAG flags)
{
	// flags is reserved for future use, and must be zero.
	CU_UNUSED(flags);
	void* data;
	VK_CHECK(vkMapMemory(m_device->native(), m_memory, offset, len, 0, &data),  "vkMapMemory fail!");
	return data;
}

void VK_Buffer::unmap()
{
	vkUnmapMemory(m_device->native(), m_memory);
}

CU_NS_END