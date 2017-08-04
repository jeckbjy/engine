//! Render
#include "Cute/RenderAPI.h"

CUTE_NS_BEGIN

IBuffer::IBuffer(const BufferDesc& desc)
    : m_usage(desc.usage)
    , m_size(desc.size)
    , m_stride(desc.stride)
{
}

ITexture::ITexture(const TextureDesc& desc)
    : m_dimension(desc.dimension)
    , m_format(desc.format)
    , m_width(desc.width)
    , m_height(desc.height)
    , m_depth(desc.depthOrArraySize)
    , m_mipmaps(desc.mipLevels)
{
}

//
//
//
RenderModule::RenderModule()
    : m_device(NULL)
{
}

RenderModule::~RenderModule()
{
    
}

void RenderModule::addFactory(IDeviceFactory *factory)
{
    m_factories.push_back(factory);
}

void RenderModule::delFactory(IDeviceFactory *factory)
{
    FactoryArray::iterator itor = std::find(m_factories.begin(), m_factories.end(), factory);
    if(itor != m_factories.end())
    {
        m_factories.erase(itor);
    }
}

void RenderModule::setDevice(const String &name)
{
    IDeviceFactory* factory = getFactory(name);
    if(factory != NULL)
    {
//        factory->getDevice(<#const Cute::DeviceDesc &desc#>, &m_device)
    }
}

IDeviceFactory* RenderModule::getFactory(const String &name)
{
    for (size_t i = 0; i < m_factories.size(); ++i) {
        if(m_factories[i]->name() == name)
            return m_factories[i];
    }
    
    return NULL;
}

IDevice* RenderModule::getDevice()
{
    if(m_device == NULL)
    {
        if(!m_factories.empty())
            m_device = m_factories[0]->create();
    }
    
    assert(m_device != NULL);
    return m_device;
}

CUTE_NS_END
