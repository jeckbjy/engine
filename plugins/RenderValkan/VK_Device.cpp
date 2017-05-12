#include "VK_Device.h"
#include "VK_Buffer.h"
#include "VK_Texture.h"
//#include "VK_Program.h"
//#include "VK_Pipeline.h"
//#include "VK_VertexLayout.h"
//#include "VK_FrameBuffer.h"
//#include "VK_SwapChain.h"
//#include "VK_DescriptorSet.h"
#include "VK_CommandList.h"
#include "Vk_CommandQueue.h"

CUTE_NS_BEGIN

#if defined(VK_USE_PLATFORM_WIN32_KHR)
#	define VK_KHR_OS_SURFACE_EXTENSION_NAME VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
#	define VK_KHR_OS_SURFACE_EXTENSION_NAME VK_KHR_ANDROID_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
#	define VK_KHR_OS_SURFACE_EXTENSION_NAME VK_KHR_XLIB_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_XCB_KHR)
#	define VK_KHR_OS_SURFACE_EXTENSION_NAME VK_KHR_XCB_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_VI_NN)
#	define VK_KHR_OS_SURFACE_EXTENSION_NAME VK_NN_VI_SURFACE_EXTENSION_NAME
#endif

template<typename T, typename DESC>
inline T* vkCreate(VK_Device* device, const DESC& desc)
{
	T* instance = new T();

	if (!instance->init(device, desc))
	{
		delete instance;
		return NULL;
	}

	return instance;
}

template<typename T>
inline T vkGetProc(VkInstance instance, const char* name)
{
	return reinterpret_cast<T>(vkGetInstanceProcAddr(instance, name));
}

template<typename T>
inline T vkGetProc(VkDevice device, const char* name)
{
	return reinterpret_cast<T>(vkGetDeviceProcAddr(device, name));
}

void vkCheckInstanceExtension
	(
		const char*			layer, 
		size_t				requestCount,
		const char**		requestNames,
		std::vector<char*>& result
	)
{
	uint32_t count;
	vkEnumerateInstanceExtensionProperties(layer, &count, nullptr);

	std::vector<VkExtensionProperties> temp;
	temp.resize(count);

	vkEnumerateInstanceExtensionProperties(layer, &count, temp.data());

	result.reserve(count);
	for (size_t i = 0; i < temp.size(); ++i)
	{
		bool hit = false;
		for (size_t j = 0; j < requestCount; ++j)
		{
			if (strcmp(temp[i].extensionName, requestNames[j]) == 0)
			{
				hit = true;
				break;
			}
		}

		if (!hit)
		{
			continue;
		}

		auto extname = new char[VK_MAX_EXTENSION_NAME_SIZE];
		memset(extname, 0, sizeof(char) * VK_MAX_EXTENSION_NAME_SIZE);
		memcpy(extname, temp[i].extensionName, sizeof(char) * VK_MAX_EXTENSION_NAME_SIZE);
		result.push_back(extname);
	}

	result.shrink_to_fit();
	temp.clear();
}

VkDebugReportCallbackEXT            vkDebugReportCallback			= VK_NULL_HANDLE;
PFN_vkCreateDebugReportCallbackEXT  vkCreateDebugReportCallback		= NULL;
PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallback	= NULL;
PFN_vkDebugReportMessageEXT         vkDebugReportMessage			= NULL;
//////////////////////////////////////////////////////////////////////////
// Degub Report
//////////////////////////////////////////////////////////////////////////
void OutputLog(const char* format, ...)
{
#if _WIN32
	char temp[2048];
	va_list va;
	va_start(va, format);
	vsprintf_s(temp, format, va);
	va_end(va);

	OutputDebugStringA(temp);
	printf_s("%s", temp);
#else
	va_list va;
	va_start(va, format);
	vprintf(format, va);
	va_end(va);
#endif
}

VKAPI_ATTR VkBool32 VKAPI_CALL DebugReport
(
VkFlags                     msgFlags,
VkDebugReportObjectTypeEXT  objType,
uint64_t                    srcObject,
size_t                      location,
int32_t                     msgCode,
const char*                 pLayerPrefix,
const char*                 pMsg,
void*                       pUserData
)
{
	CUTE_UNUSED(objType);
	CUTE_UNUSED(srcObject);
	CUTE_UNUSED(location);
	CUTE_UNUSED(pUserData);

	if (msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
	{
		OutputLog("Error [%s] Code %d : ", pLayerPrefix, msgCode);
	}
	else if (msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
	{
		OutputLog("Warning [%s] Code %d : ", pLayerPrefix, msgCode);
	}
	else if (msgFlags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
	{
		OutputLog("Information [%s] Code %d : ", pLayerPrefix, msgCode);
	}
	else if (msgFlags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
	{
		OutputLog("Debug [%s] Code %d : ", pLayerPrefix, msgCode);
	}
	else if (msgFlags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
	{
		OutputLog("Performance Warning [%s] Code %d : ", pLayerPrefix, msgCode);
	}

	OutputLog(pMsg);
	OutputLog("\n");

	return VK_TRUE;
}
//////////////////////////////////////////////////////////////////////////
// VK_Device
//////////////////////////////////////////////////////////////////////////
VK_Device::VK_Device()
	: m_instance(VK_NULL_HANDLE)
	, m_device(VK_NULL_HANDLE)
	, m_graphicsQueue(NULL)
	, m_computeQueue(NULL)
	, m_copyQueue(NULL)
{
}

VK_Device::~VK_Device()
{
	term();
}

bool VK_Device::createInstance(bool enableDebug)
{
	VkResult ret;

	const char* instanceExtension[] =
	{
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_OS_SURFACE_EXTENSION_NAME,
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME
	};

	const char* layerNames[] =
	{
		"VK_LAYER_LUNARG_standard_validation",
	};

	uint32_t instanceExtensionCount = 2;
	uint32_t layerCount = 0;

	if (enableDebug)
	{
		layerCount++;
		instanceExtensionCount++;
	}

	std::vector<char*> extensions;
	vkCheckInstanceExtension(NULL, instanceExtensionCount, instanceExtension, extensions);

	// create instance
	VkApplicationInfo appInfo = {};
	appInfo.sType					= VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext					= NULL;
	appInfo.pApplicationName		= ENGINE_NAME;
	appInfo.applicationVersion		= VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName				= ENGINE_NAME;
	appInfo.engineVersion			= VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion				= VK_API_VERSION_1_0;

	VkInstanceCreateInfo instInfo = {};
	instInfo.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instInfo.pNext					= NULL;
	instInfo.flags					= 0;
	instInfo.pApplicationInfo		= &appInfo;
	instInfo.enabledLayerCount		= layerCount;
	instInfo.ppEnabledLayerNames	= (layerCount == 0) ? NULL : layerNames;
	instInfo.enabledExtensionCount	= static_cast<uint32_t>(extensions.size());
	instInfo.ppEnabledExtensionNames= extensions.data();

	// TODO:
	//m_allocator.pfnAllocation			= Alloc;
	//m_allocator.pfnFree					= Free;
	//m_allocator.pfnReallocation			= Realloc;
	//m_allocator.pfnInternalAllocation	= NULL;
	//m_allocator.pfnInternalFree			= NULL;
	//m_allocator.pUserData				= NULL;

	ret = vkCreateInstance(&instInfo, NULL, &m_instance);

	for (size_t i = 0; i < extensions.size(); ++i)
	{
		delete[] extensions[i];
	}

	extensions.clear();

	return ret == VK_SUCCESS;
}

bool VK_Device::createPhysical()
{
	VkResult ret;
	uint32_t count = 0;
	ret = vkEnumeratePhysicalDevices(m_instance, &count, NULL);
	if (ret != VK_SUCCESS || count < 1)
		return false;

	m_physicals.resize(count);
	VkPhysicalDevice* gpus = new VkPhysicalDevice[count];
	if (gpus == NULL)
		return false;

	ret = vkEnumeratePhysicalDevices(m_instance, &count, gpus);
	if (ret != VK_SUCCESS)
	{
		delete[] gpus;
		return false;
	}

	for (size_t i = 0; i < count; ++i)
	{
		PhysicalInfo& physical = m_physicals[i];
		physical.device = gpus[i];
		vkGetPhysicalDeviceProperties(gpus[i], &physical.deviceProperties);
		vkGetPhysicalDeviceMemoryProperties(gpus[i], &physical.memoryProperties);
	}

	delete[] gpus;
	return true;
}

bool VK_Device::createDevice(const DeviceDesc& desc)
{
	const char* layerNames[] =
	{
		"VK_LAYER_LUNARG_standard_validation",
	};

	uint32_t layerCount = 0;

	if (desc.enableDebug)
		layerCount++;

	VkPhysicalDevice physicalDevice = m_physicals[0].device;

	uint32_t propCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &propCount, NULL);

	VkQueueFamilyProperties* pProps = new VkQueueFamilyProperties[propCount];
	if (pProps == NULL)
		return false;

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &propCount, pProps);

	VkDeviceQueueCreateInfo* pQueueInfos = new VkDeviceQueueCreateInfo[propCount];
	if (pQueueInfos == NULL)
	{
		delete[] pProps;
		return false;
	}

	uint32_t graphicsIndex	= UINT32_MAX;
	uint32_t computeIndex	= UINT32_MAX;
	uint32_t transferIndex	= UINT32_MAX;

	uint32_t graphicsQueueIndex = UINT32_MAX;
	uint32_t computeQueueIndex	= UINT32_MAX;
	uint32_t transferQueueindex = UINT32_MAX;

	uint32_t queueIndex = 0u;
	uint32_t totalQueueCount = 0;
	for (uint32_t i = 0u; i < propCount; ++i)
	{
		pQueueInfos[i].sType			= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		pQueueInfos[i].pNext			= NULL;
		pQueueInfos[i].flags			= 0;
		pQueueInfos[i].queueCount		= pProps[i].queueCount;
		pQueueInfos[i].queueFamilyIndex = i;

		totalQueueCount += pProps[i].queueCount;

		if (pProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			if (graphicsIndex == UINT32_MAX)
			{
				graphicsIndex = i;
				graphicsQueueIndex = queueIndex;
				queueIndex++;
			}
		}

		if ((pProps[i].queueFlags & VK_QUEUE_COMPUTE_BIT) && 
			((pProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != VK_QUEUE_GRAPHICS_BIT))
		{
			if (computeIndex == UINT32_MAX)
			{
				computeIndex = i;
				computeQueueIndex = queueIndex;
				queueIndex++;
			}
		}

		if ((pProps[i].queueFlags & VK_QUEUE_TRANSFER_BIT) && 
			((pProps[i].queueFlags & VK_QUEUE_TRANSFER_BIT) != VK_QUEUE_GRAPHICS_BIT))
		{
			if (transferIndex == UINT32_MAX)
			{
				transferIndex = i;
				transferQueueindex = queueIndex;
				queueIndex++;
			}
		}
	}

	if (computeIndex == UINT32_MAX)
	{
		for (auto i = 0u; i < propCount; ++i)
		{
			if (pProps[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				if (computeIndex == UINT32_MAX)
				{
					computeIndex = i;
					computeQueueIndex = queueIndex;
					queueIndex++;
				}
			}
		}
	}

	if (transferIndex == UINT32_MAX)
	{
		for (auto i = 0u; i < propCount; ++i)
		{

			if (pProps[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				if (transferIndex == UINT32_MAX)
				{
					transferIndex = i;
					transferQueueindex = queueIndex;
					queueIndex++;
				}
			}
		}
	}

	float* pPriorities = new float[totalQueueCount];
	if (pPriorities == NULL)
	{
		delete[] pProps;
		delete[] pQueueInfos;
		return false;
	}

	memset(pPriorities, 0, sizeof(float) * totalQueueCount);

	uint32_t offset = 0u;
	for (uint32_t i = 0u; i < propCount; ++i)
	{
		pQueueInfos[i].pQueuePriorities = &pPriorities[offset];
		offset += pProps[i].queueCount;
	}

	std::vector<char*> deviceExtensions;

	VkDeviceCreateInfo deviceInfo = {};
	deviceInfo.sType					= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.pNext					= NULL;
	deviceInfo.queueCreateInfoCount		= propCount;
	deviceInfo.pQueueCreateInfos		= pQueueInfos;
	deviceInfo.enabledLayerCount		= layerCount;
	deviceInfo.ppEnabledLayerNames		= (layerCount == 0) ? NULL : layerNames;
	deviceInfo.enabledExtensionCount	= uint32_t(deviceExtensions.size());
	deviceInfo.ppEnabledExtensionNames	= deviceExtensions.data();
	deviceInfo.pEnabledFeatures			= NULL;

	VkResult ret = vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &m_device);
	for (size_t i = 0; i < deviceExtensions.size(); ++i)
	{
		delete[] deviceExtensions[i];
	}
	deviceExtensions.clear();

	delete[] pProps;
	delete[] pQueueInfos;
	delete[] pPriorities;

	if (ret != VK_SUCCESS)
		return false;

	// create queue
	if (!VK_CommandQueue::create(
		this,
		graphicsIndex,
		graphicsQueueIndex,
		desc.maxGraphicsQueueSubmitCount,
		&m_graphicsQueue))
		return false;

	if (!VK_CommandQueue::create(
		this,
		computeIndex,
		computeQueueIndex,
		desc.maxComputeQueueSubmitCount,
		&m_computeQueue))
		return false;

	if (!VK_CommandQueue::create(
		this,
		transferIndex,
		transferQueueindex,
		desc.maxCopyQueueSubmitCount,
		&m_copyQueue))
		return false;

#if defined(VK_EXT_debug_marker)
	if (isSupportExtension(EXT_DEBUG_MARKER))
	{
		vkDebugMarkerSetObjectTag	= vkGetProc<PFN_vkDebugMarkerSetObjectTagEXT>(m_device, "vkDebugMarkerSetObjectTagEXT");
		vkDebugMarkerSetObjectName	= vkGetProc<PFN_vkDebugMarkerSetObjectNameEXT>(m_device, "vkDebugMarkerSetObjectNameEXT");
		vkCmdDebugMarkerBegin		= vkGetProc<PFN_vkCmdDebugMarkerBeginEXT>(m_device, "vkCmdDebugMarkerBeginEXT");
		vkCmdDebugMarkerEnd			= vkGetProc<PFN_vkCmdDebugMarkerEndEXT>(m_device, "vkCmdDebugMarkerEndEXT");
		vkCmdDebugMarkerInsert		= vkGetProc<PFN_vkCmdDebugMarkerInsertEXT>(m_device, "vkCmdDebugMarkerInsert");
	}
#endif

#if defined(VK_KHR_push_descriptor)
	if (isSupportExtension(EXT_KHR_PUSH_DESCRIPTOR))
	{
		vkCmdPushDescriptorSet = vkGetProc<PFN_vkCmdPushDescriptorSetKHR>(m_device, "vkCmdPushDescriptorSetKHR");
	}
#endif

#if defined(VK_EXT_hdr_metadata)
	if (isSupportExtension(EXT_HDR_METADATA))
	{
		vkSetHdrMetadata = vkGetProc<PFN_vkSetHdrMetadataEXT>(m_device, "vkSetHdrMetadataEXT");
	}
#endif

	// set limits
	VkPhysicalDeviceLimits& limits = m_physicals[0].deviceProperties.limits;
	m_info.constantBufferMemoryAlignment	= static_cast<uint32_t>(limits.minUniformBufferOffsetAlignment);
	m_info.maxTargetWidth					= limits.maxFramebufferWidth;
	m_info.maxTargetHeight					= limits.maxFramebufferHeight;
	m_info.maxTargetArraySize				= limits.maxFramebufferLayers;
	m_info.maxColorSampleCount				= static_cast<uint32_t>(limits.framebufferColorSampleCounts);
	m_info.maxDepthSampleCount				= static_cast<uint32_t>(limits.framebufferDepthSampleCounts);
	m_info.maxStencilSampleCount			= static_cast<uint32_t>(limits.framebufferStencilSampleCounts);

	return true;
}

bool VK_Device::createDebuger()
{
	vkCreateDebugReportCallback = vkGetProc<PFN_vkCreateDebugReportCallbackEXT>(
		m_instance, "vkCreateDebugReportCallbackEXT");
	vkDestroyDebugReportCallback = vkGetProc<PFN_vkDestroyDebugReportCallbackEXT>(
		m_instance, "vkDestroyDebugReportCallbackEXT");
	vkDebugReportMessage = vkGetProc<PFN_vkDebugReportMessageEXT>(
		m_instance, "vkDebugReportMessageEXT");

	if (vkCreateDebugReportCallback != NULL &&
		vkDestroyDebugReportCallback != NULL &&
		vkDebugReportMessage != NULL)
	{
		VkFlags flags = 0;
		flags |= VK_DEBUG_REPORT_ERROR_BIT_EXT;
		flags |= VK_DEBUG_REPORT_WARNING_BIT_EXT;
		flags |= VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
		flags |= VK_DEBUG_REPORT_INFORMATION_BIT_EXT;
		flags |= VK_DEBUG_REPORT_DEBUG_BIT_EXT;

		VkDebugReportCallbackCreateInfoEXT info = {};
		info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
		info.pNext = NULL;
		info.pfnCallback = DebugReport;
		info.pUserData = NULL;
		info.flags = flags;

		VkResult ret = vkCreateDebugReportCallback(m_instance, &info, NULL, &vkDebugReportCallback);
		if (ret != VK_SUCCESS)
			return false;
	}

	return true;
}

bool VK_Device::init(const DeviceDesc& desc)
{
	if (!createInstance(desc.enableDebug))
		return false;

	if (!createPhysical())
		return false;

	if (!createDevice(desc))
		return false;

	if (!createDebuger())
		return false;

	return true;
}

void VK_Device::term()
{
	delete m_graphicsQueue;
	delete m_computeQueue;
	delete m_copyQueue;

	m_graphicsQueue = NULL;
	m_computeQueue = NULL;
	m_copyQueue = NULL;

	// free debuger
	if (vkDebugReportCallback != VK_NULL_HANDLE)
	{
		vkDestroyDebugReportCallback(m_instance, vkDebugReportCallback, NULL);
		vkDebugReportCallback = VK_NULL_HANDLE;
	}

	vkCreateDebugReportCallback = NULL;
	vkDestroyDebugReportCallback = NULL;
	vkDebugReportMessage = NULL;

	if (m_device != VK_NULL_HANDLE)
	{
		vkDeviceWaitIdle(m_device);
		vkDestroyDevice(m_device, NULL);
		m_device = VK_NULL_HANDLE;
	}

	if (m_instance != VK_NULL_HANDLE)
	{
		vkDestroyInstance(m_instance, NULL);
		m_instance = VK_NULL_HANDLE;
	}
}

bool VK_Device::alloc(VkDeviceMemory& memory, const VkMemoryRequirements& requirements, VkMemoryPropertyFlags flags)
{
	//step 1: find memory type index
	VkPhysicalDeviceMemoryProperties& memoryProperties = m_physicals[0].memoryProperties;
	uint32_t index = 0u;
	uint32_t typeBits = requirements.memoryTypeBits;
	for (uint32_t i = 0; i < VK_MAX_MEMORY_TYPES; ++i)
	{
		VkMemoryPropertyFlags propFlags = memoryProperties.memoryTypes[i].propertyFlags;
		if ((typeBits & 0x1) == 0x1)
		{
			if ((propFlags & flags) == flags)
			{
				index = i;
				return true;
			}
		}
		typeBits >>= 1;
	}

	//step 2: alloc memory
	VkMemoryAllocateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	info.pNext = NULL;
	info.allocationSize = requirements.size;
	info.memoryTypeIndex = index;

	VkResult ret = vkAllocateMemory(m_device, &info, NULL, &memory);
	return ret == VK_SUCCESS;
}

bool VK_Device::isSupportExtension(EXTENSION value) const
{
	return m_supportExt.test(value);
}

//VK_Device::VK_Device()
//	: m_instance(VK_NULL_HANDLE)
//	, m_physical(VK_NULL_HANDLE)
//	, m_device(VK_NULL_HANDLE)
//	, m_descroptorPool(VK_NULL_HANDLE)
//{
//	createInstance();
//	createPhysical();
//	createDevice();
//
//	// create pool
//	VkDescriptorPoolSize typeCounts[1];
//	//typeCounts[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//	//typeCounts[0].descriptorCount = 1;
//
//	VkDescriptorPoolCreateInfo pool_info;
//	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
//	pool_info.pNext = NULL;
//	pool_info.poolSizeCount = 1;
//	pool_info.pPoolSizes = typeCounts;
//	pool_info.maxSets = 1;
//	VK_CHECK(vkCreateDescriptorPool(m_device, &pool_info, NULL, &m_descroptorPool), "vkCreateDescriptorPool fail!");
//
//}
//
//VK_Device::~VK_Device()
//{
//	vkDestroyDevice(m_device, NULL);
//}

//void VK_Device::createInstance()
//{
//	const char *layers[] = { "VK_LAYER_LUNARG_standard_validation" };
//	const char *extensions[] = { "VK_KHR_surface", "VK_KHR_win32_surface", "VK_EXT_debug_report" };
//
//	VkApplicationInfo appInfo = {};
//	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//	appInfo.pApplicationName = NULL;
//	appInfo.pEngineName = "CuteEngine";
//
//	VkInstanceCreateInfo instInfo = {};
//	instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//	instInfo.pNext = NULL;
//	instInfo.pApplicationInfo = &appInfo;
//	instInfo.enabledLayerCount = 1;
//	instInfo.ppEnabledExtensionNames = layers;
//	instInfo.enabledExtensionCount = 3;
//	instInfo.ppEnabledExtensionNames = extensions;
//
//	VK_CHECK(vkCreateInstance(&instInfo, NULL, &m_instance), "vkCreateInstance fail!");
//}
//
//void VK_Device::createPhysical()
//{
//	uint32_t physicalCount = 0;
//	vkEnumeratePhysicalDevices(m_instance, &physicalCount, NULL);
//	if (physicalCount == 0)
//		return;
//	VkPhysicalDevice* physicals = new VkPhysicalDevice[physicalCount];
//	vkEnumeratePhysicalDevices(m_instance, &physicalCount, physicals);
//	// how to find a physical
//	m_physical = physicals[0];
//	delete[] physicals;
//
//	// Fill up the physical device memory properties: 
//	vkGetPhysicalDeviceMemoryProperties(m_physical, &m_memoryProps);
//
//	//// find queue index:first:supprot all,then:supprot graphics and compute
//	//uint32_t queueCount = 0;
//	//vkGetPhysicalDeviceQueueFamilyProperties(m_physical, &queueCount, NULL);
//	//VkQueueFamilyProperties* queueProps = new VkQueueFamilyProperties[queueCount];
//	//vkGetPhysicalDeviceQueueFamilyProperties(m_physical, &queueCount, queueProps);
//
//	//// find best queue index;
//	//uint32_t queueBits[] = 
//	//{
//	//	VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT | VK_QUEUE_SPARSE_BINDING_BIT,
//	//	VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT,
//	//	VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT,
//	//	VK_QUEUE_GRAPHICS_BIT,
//	//};
//
//	//m_queueIndex = UINT32_MAX;
//
//	//uint32_t count = sizeof(queueBits) / sizeof(uint32_t);
//	//for (uint32_t i = 0; i < count; ++i)
//	//{
//	//	uint32_t flags = queueBits[i];
//	//	for (uint32_t j = 0; j < queueCount; ++j)
//	//	{
//	//		if ((queueProps[j].queueFlags & flags) == flags)
//	//		{
//	//			m_queueIndex = j;
//	//			break;
//	//		}
//	//	}
//
//	//	if (m_queueIndex != UINT32_MAX)
//	//		break;
//	//}
//}
//
//void VK_Device::createDevice()
//{
//	VkDeviceQueueCreateInfo queueInfo = {};
//	queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//	queueInfo.queueFamilyIndex = 0;
//
//	VkDeviceCreateInfo deviceInfo = {};
//	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
//	deviceInfo.queueCreateInfoCount = 1;
//	deviceInfo.pQueueCreateInfos = &queueInfo;
//
//	VK_CHECK(vkCreateDevice(m_physical, &deviceInfo, NULL, &m_device), "vkCreateDevice fail!");
//}

//
//GpuBuffer* VK_Device::newBuffer(const BufferDesc& desc)
//{
//	return new VK_Buffer(this, desc);
//}
//
//Texture* VK_Device::newTexture(const TextureDesc& desc)
//{
//	return new VK_Texture(this, desc);
//}
//
//VertexLayout* VK_Device::newVertexLayout(const VertexElement* elements, size_t count)
//{
//	return new VK_VertexLayout(newID(), elements, count);
//}
//
//ShaderStage* VK_Device::newShader()
//{
//	return new VK_Shader(newID(), this);
//}
//
//ShaderProgram* VK_Device::newProgram()
//{
//	return new VK_Program(newID());
//}
//
//Pipeline* VK_Device::newPipeline(const PipelineDesc& desc)
//{
//	VK_Program* prog = (VK_Program*)desc.program;
//	if (!prog)
//		return NULL;
//
//	if (prog->isCompute())
//		return new VK_ComputePipeline(m_device, desc);
//	else
//		return new VK_GraphicsPipeline(m_device, desc);
//}
//
//DescriptorSet* VK_Device::newDescriptorSet(Pipeline* pipeline)
//{
//	return NULL;
//}
//
//CommandBuffer* VK_Device::newCommandBuffer()
//{
//	return new VK_CommandList();
//}
//
//CommandQueue* VK_Device::newCommandQueue()
//{
//	return new VK_CommandQueue();
//}
//
//FrameBuffer* VK_Device::newFrameBuffer()
//{
//	return new VK_FrameBuffer();
//}
//
//SwapChain* VK_Device::newSwapChain(const SwapChainDesc& desc)
//{
//	return new VK_SwapChain(desc);
//}

CUTE_NS_END
