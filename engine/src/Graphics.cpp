#include "Graphics.h"

CU_NS_BEGIN

GpuBuffer::GpuBuffer(const BUFFER_DESC& desc)
: m_flags(desc.flags)
, m_bytes(desc.counts * desc.stride)
, m_count(desc.counts)
, m_stride(desc.stride)
{
}

GpuBuffer::~GpuBuffer()
{

}

CU_NS_END
