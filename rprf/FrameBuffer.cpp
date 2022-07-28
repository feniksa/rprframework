#include "FrameBuffer.h"
#include "Error.h"
#include <cassert>


namespace rprf
{

FrameBuffer::FrameBuffer(Context& context, const int width, const int height, int _numComponets, ComponentsType _type)
: m_context(context),
  m_width(width),
  m_height(height),
  m_numComponents(_numComponets),
  m_componentType(_type)
{
	int status;
	rpr_framebuffer_format format;

	assert(m_numComponents >= 3);
	assert(m_numComponents <= 4);

	format.num_components = m_numComponents;
	format.type = static_cast<unsigned int>(m_componentType);

	rpr_framebuffer_desc desc;
	desc.fb_width = width;
	desc.fb_height = height;

	rpr_framebuffer frame_buffer;
	status = rprContextCreateFrameBuffer(context.instance(), format, &desc, &frame_buffer);
	check(status);

	setInstance(std::move(frame_buffer));
}

void FrameBuffer::clear()
{
	int status;
	status = rprFrameBufferClear(instance());
	check(status);
}

void FrameBuffer::saveToFile(const std::filesystem::path& path) const
{
	int status;
	status = rprFrameBufferSaveToFile(instance(), path.string().c_str());
	check(status);
}

std::size_t FrameBuffer::bufferSize() const
{
	size_t size;
	int status;
	status = rprFrameBufferGetInfo(instance(), RPR_FRAMEBUFFER_DATA, 0, nullptr, &size);
	check(status);
	return size;
}

size_t FrameBuffer::data(void* buffer, size_t size) const
{
	int status;


	size_t newSize;
	status = rprFrameBufferGetInfo(instance(), RPR_FRAMEBUFFER_DATA, size, buffer, &newSize);
	check(status);

#ifndef NDEBUG
	size_t dd  = bufferSize();
	assert(dd == newSize);
#endif

	return newSize;
}

void FrameBuffer::data(std::vector<std::byte>* buffer) const
{
	int status;
	size_t size = bufferSize();

	if (buffer->size() != size)
		buffer->resize(size);

	status = rprFrameBufferGetInfo(instance(), RPR_FRAMEBUFFER_DATA, buffer->size(), buffer->data(), &size);
	check(status);
}

FrameBuffer FrameBuffer::clone() const
{
	FrameBuffer clone = FrameBuffer(m_context, m_width, m_height, m_numComponents, m_componentType);
	return clone;
}

} // namespace
