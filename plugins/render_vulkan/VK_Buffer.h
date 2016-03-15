#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class CU_VK_API VK_Buffer : public GpuBuffer
{
	CU_NONCOPYABLE(VK_Buffer);
public:
	VK_Buffer(VK_Device* device, const BufferDesc& desc);
	~VK_Buffer();

	void* map(size_t offset, size_t len, MAP_FLAG flags);
	void unmap();

	VkIndexType getIndexType() const { return isIndex16() ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32; }
	VkBuffer native() { return m_buffer; }

private:
	VK_Device*		m_device;
	VkBuffer		m_buffer;
	VkDeviceMemory	m_memory;
};

CU_NS_END
