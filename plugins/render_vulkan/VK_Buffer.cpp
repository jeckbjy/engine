#include "VK_Buffer.h"
#include "VK_Mapping.h"

CU_NS_BEGIN

VKBuffer::VKBuffer(VkDevice device, const BUFFER_DESC& desc)
	: GpuBuffer(desc)
{
	m_device = device;
	VkBufferCreateInfo info;
	memset(&info, 0, sizeof(info));
	info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	info.usage = VKMapping::getBufferUsage(desc.type);
	info.size = m_bytes;
	VK_CHECK(vkCreateBuffer(device, &info, NULL, &m_buffer));
}

VKBuffer::~VKBuffer()
{
	vkDestroyBuffer(m_device, m_buffer, NULL);
}

CU_NS_END
