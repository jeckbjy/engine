#pragma once
#include "VK_API.h"
#include "Cute/DynamicArray.h"

CUTE_NS_BEGIN

// ²ÎÊý
class VK_Device;
class VK_DescriptorSetLayout;
class CUTE_VK_API VK_DescriptorSet : public IDescriptorSet
{
public:
	VK_DescriptorSet();
	~VK_DescriptorSet();

	bool init(VK_Device* device, VK_DescriptorSetLayout* layout);
	void term();

	void update();

private:
	struct DescriptorWriter
	{
		VkWriteDescriptorSet		writer;
		union
		{
			VkDescriptorImageInfo	image;
			VkDescriptorBufferInfo	buffer;
			//VkBufferView			bufferView;
		};
	};

	typedef DynamicArray<DescriptorWriter> WriterArray;


	VK_Device*				m_device;
	VkDescriptorSet			m_descriptors;
	VkDescriptorSetLayout	m_layout;
	WriterArray				m_writers;

};

CUTE_NS_END
