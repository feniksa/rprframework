#include "MemoryMap.h"
#include "Error.h"
#include <iostream>
#include <cassert>

namespace riff
{

MemoryMap::MemoryMap(rif_image image, MemoryMapType mapType)
: m_image(image)
{
    if (!m_image)
        throw std::runtime_error("MemoryMap: image is null");

    map(mapType);
}

MemoryMap::~MemoryMap() noexcept
{
    const char* errorMsg = "Can't unmap memory. Error: ";
    try {
        unmap();
    } catch (const std::exception& e) {
        std::cerr << errorMsg << e.what();
    } catch (...){
        std::cerr << errorMsg << "Unknown";
    }
}

void MemoryMap::map(MemoryMapType mapType)
{
    assert(m_image);

    unmap();

    rif_int status;

    status = rifImageMap(m_image, static_cast<int>(mapType), &m_data);
    check(status);

    if (!m_data) {
        throw std::runtime_error("MemoryMap return null pointer for data. Unexpected behaviour");
    }
}

void MemoryMap::unmap()
{
    assert(m_image);

    if (!m_data)
        return;

    rif_int status;
    status = rifImageUnmap(m_image, m_data);
    check(status);

    m_data = nullptr;
}

} // namespace riff