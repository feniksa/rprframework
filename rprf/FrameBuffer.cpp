#include "FrameBuffer.h"
#include "Error.h"
#include <cassert>


namespace rprf
{
FrameBuffer::FrameBuffer(Context& context, const FrameBufferFormat& fbFormat, const FrameBufferDesc& fbDesc):
m_context(context)
{
    int status;
    rpr_framebuffer_format format = fbFormat.format();

    assert(format.num_components >= 3);
    assert(format.num_components <= 4);

    rpr_framebuffer_desc desc = fbDesc.desc();

    rpr_framebuffer frame_buffer;
    status = rprContextCreateFrameBuffer(context.instance(), format, &desc, &frame_buffer);
    check(status);

    setInstance(std::move(frame_buffer));
}

FrameBuffer::FrameBuffer(Context& context, unsigned int width, unsigned int height, unsigned int _numComponets, ComponentsType _type)
: FrameBuffer(context,
              FrameBufferFormat {.type = _type, .numComponents = _numComponets},
              FrameBufferDesc { .width = width, .height = height })
{
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
    const auto format = getFormat();
    const auto desc = getDesc();

	FrameBuffer clone = FrameBuffer(m_context, format, desc);
	return clone;
}


FrameBufferFormat FrameBuffer::getFormat() const
{
    int status;

    rpr_framebuffer_format format;
    size_t returnBytes = 0;
    status = rprFrameBufferGetInfo(instance(), RPR_FRAMEBUFFER_FORMAT, sizeof(format), &format, &returnBytes);
    check(status);

    assert(returnBytes == sizeof(format));
    assert(format.num_components == 3 || format.num_components == 4);

    return {.type          = static_cast<ComponentsType>(format.type),
            .numComponents = format.num_components};
}

FrameBufferDesc FrameBuffer::getDesc() const
{
    int status;

    rpr_framebuffer_desc desc;
    size_t returnBytes = 0;
    status = rprFrameBufferGetInfo(instance(), RPR_FRAMEBUFFER_DESC, sizeof(desc), &desc, &returnBytes);
    check(status);

    assert(returnBytes == sizeof(desc));

    return {.width  = desc.fb_width,
            .height = desc.fb_height};
}


} // namespace
