#include "FrameBufferSysmem.h"
#include "Context.h"
#include <cassert>

namespace rprf
{

FrameBufferSysmem::FrameBufferSysmem(Context& context, const FrameBufferFormat& format, const FrameBufferDesc& desc)
: m_frameBuffer(context, format, desc),
  m_bufferVersion(0),
  m_bufferSysmemVersion(0)
{
    m_buffer.resize(m_frameBuffer.bufferSize());
    assert(m_frameBuffer.bufferSize() == format.numComponents * sizeOf(format.type) * desc.width * desc.height);
}

const FrameBuffer& FrameBufferSysmem::rprFrameBufferSysmem()  const
{
	assert(m_frameBuffer);
	return m_frameBuffer;
}

FrameBuffer& FrameBufferSysmem::rprFrameBufferSysmem()
{
	assert(m_frameBuffer);
	return m_frameBuffer;
}

bool FrameBufferSysmem::save(const std::string_view& fileName) const
{
	if (!m_frameBuffer)
		return false;

	m_frameBuffer.saveToFile(fileName);
	return true;
}

void FrameBufferSysmem::update()
{
	if (!m_frameBuffer)
		return;

	if (m_buffer.empty())
		return;

	assert(m_buffer.size()  == m_frameBuffer.bufferSize());

	size_t bufferSize = m_frameBuffer.data(m_buffer.data(), m_buffer.size() * sizeof(ContainerType::value_type));
	assert(bufferSize == m_buffer.size());

	m_bufferSysmemVersion = m_bufferVersion;
}

void FrameBufferSysmem::clear()
{
	if (m_frameBuffer)
		m_frameBuffer.clear();
}

FrameBufferFormat FrameBufferSysmem::format() const
{
    assert(m_frameBuffer);
    return m_frameBuffer.getFormat();
}

FrameBufferDesc FrameBufferSysmem::desc() const
{
    assert(m_frameBuffer);
    return m_frameBuffer.getDesc();
}

} // namespace
