#include "FrameBufferSysmem.h"
#include "Context.h"
#include <cassert>
#include "ThirdParty/stb/stb_image.h"
#include "ThirdParty/stb/stb_image_write.h"
#include "Color4.h"
#include "Error.h"

namespace rprf
{

FrameBufferSysmem::FrameBufferSysmem(const FrameBuffer& frameBuffer)
: m_frameBuffer(frameBuffer)
{
    assert(frameBuffer);
    m_buffer.resize(m_frameBuffer.bufferSize());
}

const FrameBuffer& FrameBufferSysmem::frameBuffer()  const
{
	return m_frameBuffer;
}

void float4f_to_float4b(const Color4f* src, const Color4f* end, Color4b* dst)
{
    for (;src != end; ++src, ++dst)
    {
        dst->r = src->r * 255.0f;
        dst->g = src->g * 255.0f;
        dst->b = src->b * 255.0f;
        dst->a = src->a * 255.0f;
    }
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

    std::vector<char> dstBuffer(m_buffer.size());

    const Color4f* src_begin = reinterpret_cast<const Color4f*>(m_buffer.data());
    const Color4f* src_end = src_begin + m_buffer.size() / sizeof(float) / 4;
    Color4b* dst_begin = reinterpret_cast<Color4b*>(dstBuffer.data());
    Color4b* dst_end = reinterpret_cast<Color4b*>(dstBuffer.data() + dstBuffer.size() / sizeof(char) / 4);

    if (m_buffer.size() <= 64 * 64) {
        float4f_to_float4b(src_begin, src_end, dst_begin);
    } else {
        const unsigned int threadsNum = std::thread::hardware_concurrency();

        const unsigned int srcBucketSize = std::distance(src_begin, src_end) / threadsNum;
        const unsigned int dstBucketSize = std::distance(dst_begin, dst_end) / threadsNum;

        std::vector<std::thread> threadPool;
        for (unsigned int t = 0; t < threadsNum; ++t) {
            const auto _begin = src_begin + t * srcBucketSize;
            const auto _end = std::min(_begin + srcBucketSize, src_end);
            auto _dst = dst_begin + t * dstBucketSize;

            threadPool.emplace_back(float4f_to_float4b, _begin, _end, _dst);
        }

        for (unsigned int t = 0; t < threadsNum; ++t) {
            threadPool[t].join();
        }
    }

    status = stbi_write_png(filename.c_str(), desc.width, desc.height,
                            format.numComponents, dstBuffer.data(),
                            desc.width * format.numComponents );
    return status == 1;
}

void FrameBufferSysmem::update()
{
	if (!m_frameBuffer)
		return;

	if (m_buffer.empty())
		return;

	assert(m_buffer.size()  == m_frameBuffer.bufferSize());

	size_t bufferSize = m_frameBuffer.data(m_buffer.data(), m_buffer.size() * sizeof(ContainerType::value_type));
    assert(bufferSize == m_buffer.size() * sizeof(ContainerType::value_type));
}

} // namespace
