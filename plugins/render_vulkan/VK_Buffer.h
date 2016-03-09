#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class CU_VK_API VKBuffer : public GpuBuffer
{
public:
	VKBuffer(VkDevice device, const BUFFER_DESC& desc);
	~VKBuffer();

	VkIndexType getIndexType() const { return isIndex16() ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32; }
	VkBuffer native() { return m_buffer; }

private:
	VkDevice		m_device;
	VkBuffer		m_buffer;
	VkDeviceMemory	m_memory;
};

CU_NS_END
