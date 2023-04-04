#pragma once

#include "Color4.h"
#include <thread>
#include <vector>
#include <cassert>

namespace rprf
{
    void float4f_to_float4b(const Color4f* src_begin, const Color4f* src_end, Color4b* dst_begin,Color4b* dst_end)
    {
        for (;src_begin != src_end && dst_begin != dst_end; ++src_begin, ++dst_begin)
        {
            dst_begin->r = src_begin->r * 255.0f;
            dst_begin->g = src_begin->g * 255.0f;
            dst_begin->b = src_begin->b * 255.0f;
            dst_begin->a = src_begin->a * 255.0f;

            assert(src_begin <= src_end);
            assert(dst_begin <= dst_end);
        }
    }

    void float4f_to_float4b_parallel(const Color4f* src_begin, const Color4f* src_end, Color4b* dst_begin, Color4b* dst_end)
    {
        const unsigned int threadsNum = std::thread::hardware_concurrency();

        const unsigned int srcBucketSize = std::distance(src_begin, src_end) / threadsNum;
        const unsigned int dstBucketSize = std::distance(dst_begin, dst_end) / threadsNum;

        std::vector<std::thread> threadPool;
        for (unsigned int t = 0; t < threadsNum; ++t) {
            const auto _src_begin = src_begin + t * srcBucketSize;
            const auto _src_end = std::min(_src_begin + srcBucketSize, src_end);

            auto _dst_begin = dst_begin + t * dstBucketSize;
            auto _dst_end = std::min(_dst_begin + dstBucketSize, dst_end);

            threadPool.emplace_back(float4f_to_float4b, _src_begin, _src_end, _dst_begin, _dst_end);
        }

        for (unsigned int t = 0; t < threadsNum; ++t) {
            threadPool[t].join();
        }
    }

}
