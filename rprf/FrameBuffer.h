#pragma once

#include "rprf/Context.h"
#include "rprf/ComponentType.h"
#include <vector>
#include <filesystem>

namespace rprf
{

struct FrameBufferFormat
{
    ComponentsType type;
    unsigned int numComponents;

	[[nodiscard]]
	RPRF_API
	rpr_framebuffer_format format() const { return {numComponents,static_cast<unsigned int>(type) }; }
};

struct FrameBufferDesc
{
    unsigned int width;
    unsigned int height;

	[[nodiscard]]
	RPRF_API
	rpr_framebuffer_desc desc() const { return {width, height }; }
};

class FrameBuffer : public ContextObject
{
public:
	RPRF_API
	FrameBuffer(Context& context, unsigned int width, unsigned int height, unsigned int numComponets = 4, ComponentsType type = ComponentsType::Float32);

	RPRF_API
	FrameBuffer(Context& context, const FrameBufferFormat& format, const FrameBufferDesc& desc);

	RPRF_API
	size_t data(void* buffer, size_t size) const;

	RPRF_API
	void data(std::vector<std::byte>* buffer) const;

	RPRF_API
	void saveToFile(const std::filesystem::path& path) const;

	RPRF_API
	void clear();

	[[nodiscard]]
	RPRF_API
	std::size_t bufferSize() const;

	[[nodiscard]]
	RPRF_API
    FrameBufferFormat getFormat() const;

	[[nodiscard]]
	RPRF_API
    FrameBufferDesc getDesc() const;

	[[nodiscard]]
	RPRF_API
	FrameBuffer clone() const;
private:
	Context& m_context;
};

} // namespace
