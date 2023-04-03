#pragma once

#include "Context.h"
#include "ComponentType.h"
#include <vector>
#include <filesystem>

namespace rprf
{

struct FrameBufferFormat
{
    ComponentsType type;
    unsigned int numComponents;

    rpr_framebuffer_format format() const { return {numComponents,static_cast<unsigned int>(type) }; }
};

struct FrameBufferDesc
{
    unsigned int width;
    unsigned int height;

    rpr_framebuffer_desc desc() const { return {width, height }; }
};

class FrameBuffer : public ContextObject<rpr_framebuffer>
{
public:
	FrameBuffer(Context& context, unsigned int width, unsigned int height, unsigned int numComponets = 4, ComponentsType type = ComponentsType::Float32);
    FrameBuffer(Context& context, const FrameBufferFormat& format, const FrameBufferDesc& desc);

	size_t data(void* buffer, size_t size) const;
	void data(std::vector<std::byte>* buffer) const;

	void saveToFile(const std::filesystem::path& path) const;

	void clear();

	std::size_t bufferSize() const;

    FrameBufferFormat getFormat() const;
    FrameBufferDesc getDesc() const;

	FrameBuffer clone() const;
private:
	Context& m_context;
};

} // namespace
