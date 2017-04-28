#include "VK_Device.h"
#include "VK_Buffer.h"
#include "VK_Texture.h"
#include "VK_Program.h"
#include "VK_Pipeline.h"
#include "VK_VertexLayout.h"
#include "VK_FrameBuffer.h"
#include "VK_SwapChain.h"
#include "VK_DescriptorSet.h"
#include "VK_CommandBuffer.h"
#include "Vk_CommandQueue.h"

CUTE_NS_BEGIN

VK_Device* gVKDevice()
{
	return gGraphics.getDevice()->cast<VK_Device>();
}

VkDevice gVKNativeDevice()
{
	VK_Device* device = gGraphics.getDevice()->cast<VK_Device>();
	if (device)
		return device->native();

	return VK_NULL_HANDLE;
}

VK_Device::VK_Device()
	: m_instance(VK_NULL_HANDLE)
	, m_physical(VK_NULL_HANDLE)
	, m_device(VK_NULL_HANDLE)
	, m_descroptorPool(VK_NULL_HANDLE)
{
	createInstance();
	createPhysical();
	createDevice();

	// create pool
	VkDescriptorPoolSize typeCounts[1];
	//typeCounts[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	//typeCounts[0].descriptorCount = 1;

	VkDescriptorPoolCreateInfo pool_info;
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.pNext = NULL;
	pool_info.poolSizeCount = 1;
	pool_info.pPoolSizes = typeCounts;
	pool_info.maxSets = 1;
	VK_CHECK(vkCreateDescriptorPool(m_device, &pool_info, NULL, &m_descroptorPool), "vkCreateDescriptorPool fail!");

}

VK_Device::~VK_Device()
{
	vkDestroyDevice(m_device, NULL);
}

void VK_Device::createInstance()
{
	const char *layers[] = { "VK_LAYER_LUNARG_standard_validation" };
	const char *extensions[] = { "VK_KHR_surface", "VK_KHR_win32_surface", "VK_EXT_debug_report" };

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = NULL;
	appInfo.pEngineName = "CuteEngine";

	VkInstanceCreateInfo instInfo = {};
	instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instInfo.pNext = NULL;
	instInfo.pApplicationInfo = &appInfo;
	instInfo.enabledLayerCount = 1;
	instInfo.ppEnabledExtensionNames = layers;
	instInfo.enabledExtensionCount = 3;
	instInfo.ppEnabledExtensionNames = extensions;

	VK_CHECK(vkCreateInstance(&instInfo, NULL, &m_instance), "vkCreateInstance fail!");
}

void VK_Device::createPhysical()
{
	uint32_t physicalCount = 0;
	vkEnumeratePhysicalDevices(m_instance, &physicalCount, NULL);
	if (physicalCount == 0)
		return;
	VkPhysicalDevice* physicals = new VkPhysicalDevice[physicalCount];
	vkEnumeratePhysicalDevices(m_instance, &physicalCount, physicals);
	// how to find a physical
	m_physical = physicals[0];
	delete[] physicals;

	// Fill up the physical device memory properties: 
	vkGetPhysicalDeviceMemoryProperties(m_physical, &m_memoryProps);

	//// find queue index:first:supprot all,then:supprot graphics and compute
	//uint32_t queueCount = 0;
	//vkGetPhysicalDeviceQueueFamilyProperties(m_physical, &queueCount, NULL);
	//VkQueueFamilyProperties* queueProps = new VkQueueFamilyProperties[queueCount];
	//vkGetPhysicalDeviceQueueFamilyProperties(m_physical, &queueCount, queueProps);

	//// find best queue index;
	//uint32_t queueBits[] = 
	//{
	//	VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT | VK_QUEUE_SPARSE_BINDING_BIT,
	//	VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT,
	//	VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT,
	//	VK_QUEUE_GRAPHICS_BIT,
	//};

	//m_queueIndex = UINT32_MAX;

	//uint32_t count = sizeof(queueBits) / sizeof(uint32_t);
	//for (uint32_t i = 0; i < count; ++i)
	//{
	//	uint32_t flags = queueBits[i];
	//	for (uint32_t j = 0; j < queueCount; ++j)
	//	{
	//		if ((queueProps[j].queueFlags & flags) == flags)
	//		{
	//			m_queueIndex = j;
	//			break;
	//		}
	//	}

	//	if (m_queueIndex != UINT32_MAX)
	//		break;
	//}
}

void VK_Device::createDevice()
{
	VkDeviceQueueCreateInfo queueInfo = {};
	queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueInfo.queueFamilyIndex = 0;

	VkDeviceCreateInfo deviceInfo = {};
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.queueCreateInfoCount = 1;
	deviceInfo.pQueueCreateInfos = &queueInfo;

	VK_CHECK(vkCreateDevice(m_physical, &deviceInfo, NULL, &m_device), "vkCreateDevice fail!");
}

void VK_Device::allocMemory(VkDeviceMemory& memory, uint32_t bytes, uint32_t typeBits, VkFlags properties)
{
	// find memoryTypeIndex;
	uint32_t memoryTypeIndex = 0;
	for (uint32_t i = 0; i < VK_MAX_MEMORY_TYPES; ++i)
	{
		if (typeBits & 1)
		{
			if ((m_memoryProps.memoryTypes[i].propertyFlags & properties) == properties)
			{
				memoryTypeIndex = i;
				break;
			}
		}

		typeBits >>= 1;
	}

	VkMemoryAllocateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	info.pNext = NULL;
	info.allocationSize = bytes;
	info.memoryTypeIndex = memoryTypeIndex;

	VK_CHECK(vkAllocateMemory(m_device, &info, NULL, &memory), "vkAllocateMemory fail!");
}

GpuBuffer* VK_Device::newBuffer(const BufferDesc& desc)
{
	return new VK_Buffer(this, desc);
}

Texture* VK_Device::newTexture(const TextureDesc& desc)
{
	return new VK_Texture(this, desc);
}

VertexLayout* VK_Device::newVertexLayout(const VertexElement* elements, size_t count)
{
	return new VK_VertexLayout(newID(), elements, count);
}

ShaderStage* VK_Device::newShader()
{
	return new VK_Shader(newID(), this);
}

ShaderProgram* VK_Device::newProgram()
{
	return new VK_Program(newID());
}

Pipeline* VK_Device::newPipeline(const PipelineDesc& desc)
{
	VK_Program* prog = (VK_Program*)desc.program;
	if (!prog)
		return NULL;

	if (prog->isCompute())
		return new VK_ComputePipeline(m_device, desc);
	else
		return new VK_GraphicsPipeline(m_device, desc);
}

DescriptorSet* VK_Device::newDescriptorSet(Pipeline* pipeline)
{
	return NULL;
}

CommandBuffer* VK_Device::newCommandBuffer()
{
	return new VK_CommandBuffer();
}

CommandQueue* VK_Device::newCommandQueue()
{
	return new VK_CommandQueue();
}

FrameBuffer* VK_Device::newFrameBuffer()
{
	return new VK_FrameBuffer();
}

SwapChain* VK_Device::newSwapChain(const SwapChainDesc& desc)
{
	return new VK_SwapChain(desc);
}

CUTE_NS_END
