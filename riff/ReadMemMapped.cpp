#include "ReadMemMapped.h"
#include "Error.h"
#include <iostream>
#include <cassert>

namespace riff
{

ReadMemMapped::ReadMemMapped(rif_image image)
: m_image(image)
{
    if (!m_image)
        throw std::runtime_error("ReadMemMapped: image is null");

    map();
}

ReadMemMapped::~ReadMemMapped() noexcept
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

void ReadMemMapped::map()
{
    assert(m_image);

    unmap();

    rif_int status;

    status = rifImageMap(m_image, RIF_IMAGE_MAP_READ, &m_data);
    check(status);

    if (!m_data) {
        throw std::runtime_error("ReadMemMapped return null pointer for data. Unexpected behaviour");
    }
}

void ReadMemMapped::unmap()
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