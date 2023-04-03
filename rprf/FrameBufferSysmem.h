#pragma once

#include "FrameBuffer.h"
#include <vector>
#include <memory>

namespace rprf
{

class Context;

class FrameBufferSysmem
{
public:
	using ContainerType = std::vector<std::byte>;

	explicit FrameBufferSysmem(rprf::Context& context, const FrameBufferFormat& format, const FrameBufferDesc& desc);

	void update();
	void clear();
	void reset();

	void markDirty()         noexcept { ++m_bufferVersion; }
	bool isDirty()     const noexcept { return m_bufferVersion != m_bufferSysmemVersion; }
	void invalidateCounter() noexcept { m_bufferVersion = 0; m_bufferSysmemVersion = 0; }

	bool save(const std::string_view& fileName) const;

	FrameBufferFormat format() const;
	FrameBufferDesc desc() const;

	const FrameBuffer& rprFrameBufferSysmem() const;
	FrameBuffer& rprFrameBufferSysmem();

	      ContainerType& buffer()       { return m_buffer; }
	const ContainerType& buffer() const { return m_buffer; }
private:
	FrameBuffer m_frameBuffer;
	ContainerType m_buffer;

	unsigned int m_bufferVersion;
	unsigned int m_bufferSysmemVersion;
};

} // namespace
