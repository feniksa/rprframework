#include "FrameBufferSysmem.h"
#include "Context.h"
#include <cassert>

namespace rprf
{

FrameBufferSysmem::FrameBufferSysmem(Context& context, unsigned int width, unsigned int height)
: m_context(context),
  m_width(width),
  m_height(height),
  m_bufferVersion(0),
  m_bufferSysmemVersion(0)
{
	resize(m_width, m_height);
}

void FrameBufferSysmem::resize(unsigned int frameWidth, unsigned int frameHeight)
{
	m_width = frameWidth;
	m_height = frameHeight;

	m_frameBuffer = std::make_unique<FrameBuffer>(m_context, m_width, m_height);
	size_t colorSize = m_frameBuffer->numComponents() * sizeOf(m_frameBuffer->componentType());

	m_buffer.resize(m_width * m_height * colorSize);
}

const FrameBuffer& FrameBufferSysmem::rprFrameBufferSysmem()  const
{
	assert(m_frameBuffer);
	return *m_frameBuffer;
}

FrameBuffer& FrameBufferSysmem::rprFrameBufferSysmem()
{
	assert(m_frameBuffer);
	return *m_frameBuffer;
}

bool FrameBufferSysmem::save(const std::string_view& fileName) const
{
	if (!m_frameBuffer)
		return false;

	m_frameBuffer->saveToFile(fileName);
	return true;
}

void FrameBufferSysmem::update()
{
	if (!m_frameBuffer)
		return;

	if (m_buffer.empty())
		return;

	assert(m_buffer.size()  == m_frameBuffer->bufferSize());

	size_t bufferSize = m_frameBuffer->data(m_buffer.data(), m_buffer.size() * sizeof(BufferType::value_type));
	assert(bufferSize == m_buffer.size());

	m_bufferSysmemVersion = m_bufferVersion;
}

void FrameBufferSysmem::clear()
{
	if (m_frameBuffer)
		m_frameBuffer->clear();
}

void FrameBufferSysmem::reset()
{
	m_frameBuffer.reset();
	m_buffer.clear();
}

int FrameBufferSysmem::numComponents() const
{
	if (!m_frameBuffer)
		return 0;

	return m_frameBuffer->numComponents();
}

ComponentsType FrameBufferSysmem::componentsType() const
{
	assert(m_frameBuffer);
	return m_frameBuffer->componentType();
}

} // namespace
