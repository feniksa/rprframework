#pragma once

#include <RadeonImageFilters.h>

namespace riff
{

    enum class MemoryMapType {
        Read = RIF_IMAGE_MAP_READ,
        Write = RIF_IMAGE_MAP_WRITE,
    };

class MemoryMap final
{
public:
    MemoryMap(rif_image image, MemoryMapType mapType);
    ~MemoryMap() noexcept;

    void unmap();

    inline void *data() const noexcept { return m_data; }

    MemoryMap(const MemoryMap &) = delete;
    MemoryMap &operator=(const MemoryMap &) = delete;

private:
    void map(MemoryMapType mapType);

    rif_image m_image;
    void *m_data;
};

}