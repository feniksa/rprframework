#pragma once

#include "FrameBuffer.h"
#include <vector>
#include <memory>
#include <boost/noncopyable.hpp>

namespace rprf
{

class Context;

class FrameBufferSysmem
{
public:
	using ContainerType = std::vector<std::byte>;

	explicit FrameBufferSysmem(const FrameBuffer& frameBuffer);

	bool saveToFile(const std::filesystem::path& fileName) const;

	void update();

	const FrameBuffer& frameBuffer() const;

	      ContainerType& data()       { return m_buffer; }
	const ContainerType& data() const { return m_buffer; }
private:
	const FrameBuffer& m_frameBuffer;
	ContainerType m_buffer;
};

} // namespace
