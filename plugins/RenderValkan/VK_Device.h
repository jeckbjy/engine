#pragma once
#include "VK_API.h"
#include "Cute/DynamicArray.h"
#include "Cute/BitMask.h"

CUTE_NS_BEGIN

class VK_CommandQueue;
class CUTE_VK_API VK_Device : public IDevice
{
public:
	enum EXTENSION
	{
		EXT_KHR_PUSH_DESCRIPTOR				= 0x01,	// VK_KHR_push_descriptor
		EXT_KHR_DESCRIPTOR_UPDATE_TEMPLATE	= 0x02,	// VK_KHR_descriptor_upate_template
		EXT_NVX_DEVICE_GENERATE_COMMAND		= 0x04,	// VK_NVX_device_generate_command
		EXT_AMD_DRAW_INDIRECT_COUNT			= 0x08,	// VK_AMD_draw_indirect_count
		EXT_DEBUG_MARKER					= 0x10,	// VK_EXT_debug_marker
		EXT_HDR_METADATA					= 0x20,	// VK_EXT_hdr_metadata
	};

	VK_Device();
	virtual ~VK_Device();

	bool init(const DeviceDesc& desc);
	void term();

	//GpuBuffer*			newBuffer(const BufferDesc& desc);
	//Texture*			newTexture(const TextureDesc& desc);
	//VertexLayout*		newVertexLayout(const VertexElement* elements, size_t count);
	//ShaderStage*		newShader();
	//ShaderProgram*		newProgram();
	//Pipeline*			newPipeline(const PipelineDesc& desc);
	//DescriptorSet*		newDescriptorSet(Pipeline* pipeline);
	//CommandBuffer*		newCommandBuffer();
	//CommandQueue*		newCommandQueue();
	//FrameBuffer*		newFrameBuffer();
	//SwapChain*			newSwapChain(const SwapChainDesc& desc);

	bool alloc(VkDeviceMemory& memory, const VkMemoryRequirements& requirements, VkMemoryPropertyFlags flags);
	bool isSupportExtension(EXTENSION value) const;

	VkDevice			getDevice() const;
	size_t				getPhysicalDeviceCount() const;
	VkPhysicalDevice	getPhysicalDevice(size_t index = 0) const;
	VkPhysicalDeviceMemoryProperties getMemoryProperties(size_t index) const;

private:
	bool createInstance(bool enableDebug);
	bool createPhysical();
	bool createDevice(const DeviceDesc& desc);
	bool createDebuger();

private:
	struct PhysicalInfo
	{
		VkPhysicalDevice					device;
		VkPhysicalDeviceProperties			deviceProperties;
		VkPhysicalDeviceMemoryProperties	memoryProperties;
	};

	typedef DynamicArray<PhysicalInfo> PhysicalArray;
	DeviceInfo				m_info;
	VkInstance				m_instance;
	VkDevice				m_device;
	PhysicalArray			m_physicals;
	VkAllocationCallbacks	m_allocator;
	BitMask32				m_supportExt;
	VK_CommandQueue*		m_graphicsQueue;
	VK_CommandQueue*		m_computeQueue;
	VK_CommandQueue*		m_copyQueue;
};

CUTE_NS_END
