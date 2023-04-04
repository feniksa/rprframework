#pragma once

namespace rprf
{
    struct Color4b
    {
        char r;
        char g;
        char b;
        char a;
    };
    struct Color4f
    {
        float r;
        float g;
        float b;
        float a;
    };

    void float4f_to_float4b(const Color4f* src_begin, const Color4f* src_end, Color4b* dst_begin, Color4b* dst_end);
    void float4f_to_float4b_parallel(const Color4f* src_begin, const Color4f* src_end, Color4b* dst_begin,Color4b* dst_end);
}
