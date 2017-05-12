#pragma once
#include "VK_API.h"

CUTE_NS_BEGIN

class VK_Device;
class CUTE_VK_API VK_Buffer : public IBuffer
{
public:
	VK_Buffer();
	~VK_Buffer();

	bool init(VK_Device* device, const BufferDesc& desc);
	void term();

	void* map();
	void  unmap();

	//VkIndexType getIndexType() const { return isIndex16() ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32; }
	VkBuffer	native() { return m_buffer; }

private:
	VK_Device*				m_device;
	VkBuffer				m_buffer;
	VkDeviceMemory			m_memory;
	RESOURCE_STATE			m_state;
	uint64					m_size;
};

CUTE_NS_END
