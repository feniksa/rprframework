#include "rprf/FrameBufferSysmem.h"
#include "rprf/Context.h"
#include "rprf/Color4.h"
#include "rprf/Error.h"

#include "ThirdParty/stb/stb_image.h"
#include "ThirdParty/stb/stb_image_write.h"

#include <cassert>

namespace rprf
{

FrameBufferSysmem::FrameBufferSysmem(const FrameBuffer& frameBuffer)
: m_frameBuffer(frameBuffer)
{
    assert(m_frameBuffer.instance());
    m_buffer.resize(m_frameBuffer.bufferSize());
}

const FrameBuffer& FrameBufferSysmem::frameBuffer()  const
{
	return m_frameBuffer;
}

bool FrameBufferSysmem::saveToFile(const std::filesystem::path& fileName) const
{
    int status;
    const auto format = m_frameBuffer.getFormat();
    const auto desc = m_frameBuffer.getDesc();
    const std::string filename = fileName.string();

    if (format.type != ComponentsType::Float32)
        throw Error(RPR_ERROR_INTERNAL_ERROR);
    if (format.numComponents != 4)
        throw Error(RPR_ERROR_INTERNAL_ERROR);

    static_assert(sizeof(float) == 4);

    std::vector<std::byte> dstBuffer(m_buffer.size() / 4); // rgba, 1 byte for each channel

    const auto src_begin = reinterpret_cast<const Color4f*>(m_buffer.data());
    const auto src_end = reinterpret_cast<const Color4f*>(m_buffer.data() + m_buffer.size());

    auto dst_begin = reinterpret_cast<Color4b*>(dstBuffer.data());
    auto dst_end = reinterpret_cast<Color4b*>(dstBuffer.data() + dstBuffer.size());

    if (m_buffer.size() <= 64 * 64) {
        float4f_to_float4b(src_begin, src_end, dst_begin, dst_end);
    } else {
        float4f_to_float4b_parallel(src_begin, src_end, dst_begin, dst_end);
    }

    status = stbi_write_png(filename.c_str(), desc.width, desc.height,
                            format.numComponents, dstBuffer.data(),
                            desc.width * format.numComponents );
    return status == 1;
}

void FrameBufferSysmem::update()
{
	if (!m_frameBuffer.instance())
		return;

	if (m_buffer.empty())
		return;

	assert(m_buffer.size()  == m_frameBuffer.bufferSize());

	size_t bufferSize = m_frameBuffer.data(m_buffer.data(), m_buffer.size() * sizeof(ContainerType::value_type));
    assert(bufferSize == m_buffer.size() * sizeof(ContainerType::value_type));
}

} // namespace
