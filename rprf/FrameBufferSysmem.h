#pragma once

#include "rprf/FrameBuffer.h"
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

	RPRF_API
	explicit FrameBufferSysmem(const FrameBuffer& frameBuffer);

	[[nodiscard]]
	RPRF_API
	bool saveToFile(const std::filesystem::path& fileName) const;

	RPRF_API
	void update();

	[[nodiscard]]
	RPRF_API
	const FrameBuffer& frameBuffer() const;

	RPRF_API
	ContainerType& data()       { return m_buffer; }

	[[nodiscard]]
	RPRF_API
	const ContainerType& data() const { return m_buffer; }
private:
	const FrameBuffer& m_frameBuffer;
	ContainerType m_buffer;
};

} // namespace
