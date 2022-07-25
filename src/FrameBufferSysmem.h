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
	using BufferType = std::vector<std::byte>;

	FrameBufferSysmem(rprf::Context& context, unsigned int width = 0, unsigned int height = 0);

	void resize(unsigned int width, unsigned int height);
	void update();
	void clear();
	void reset();

	void markDirty()         noexcept { ++m_bufferVersion; }
	bool isDirty()     const noexcept { return m_bufferVersion != m_bufferSysmemVersion; }
	void invalidateCounter() noexcept { m_bufferVersion = 0; m_bufferSysmemVersion = 0; }

	bool save(const std::wstring& fileName) const;

	unsigned int width()  const noexcept { return m_width;  }
	unsigned int height() const noexcept { return m_height; }

	int numComponents() const;
	ComponentsType componentsType() const;

	const FrameBuffer& rprFrameBufferSysmem() const;
	FrameBuffer& rprFrameBufferSysmem();

	      BufferType& buffer()       { return m_buffer; }
	const BufferType& buffer() const { return m_buffer; }
private:
	Context& m_context;
	std::unique_ptr<FrameBuffer> m_frameBuffer;

	BufferType m_buffer;
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_bufferVersion;
	unsigned int m_bufferSysmemVersion;
};

} // namespace
