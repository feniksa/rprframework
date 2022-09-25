#pragma once

#include <RadeonImageFilters.h>

namespace riff
{

class ReadMemMapped final
{
public:
    ReadMemMapped(rif_image image);
    ~ReadMemMapped() noexcept;

    void unmap();

    inline void *data() const noexcept { return m_data; }

    ReadMemMapped(const ReadMemMapped &) = delete;
    ReadMemMapped &operator=(const ReadMemMapped &) = delete;

private:
    void map();

    rif_image m_image;
    void *m_data;
};

}