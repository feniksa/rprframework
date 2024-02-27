#include "Utils.h"
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <cassert>
#include <cstring>

MemoryImage::MemoryImage(const std::filesystem::path& path)
{
    if (!load(path))
        throw std::runtime_error("Can't load image: " + path.string());
}

MemoryImage::MemoryImage(int width, int height, int channels_num) :
m_width(width),
m_height(height),
m_channels_num(channels_num),
m_buffer(new unsigned char[std::max(0, size())])
{
}

bool MemoryImage::load(const std::filesystem::path& path)
{
    stbi_uc* pixels = stbi_load(path.string().c_str(), &m_width, &m_height, &m_channels_num, 0);
    if (!pixels) {
        return false;
    }
    m_buffer.reset(pixels);

    return true;
}

bool MemoryImage::save(const std::filesystem::path& path) const
{
    int status;
    status = stbi_write_png(path.string().c_str(), m_width, m_height, m_channels_num, m_buffer.get(), m_width * m_channels_num);
    if (status == 0)
        return false;

    return true;
}

void MemoryImage::clear()
{
    if (!m_buffer)
        return;

    std::memset(m_buffer.get(), 0, size());
}

bool operator==(const MemoryImage& image1, const MemoryImage& image2)
{
    if (!image1)
        return false;
    if (!image2)
        return false;

    if (image1.width() != image2.width())
        return false;
    if (image1.height() != image2.height())
        return false;
    if (image1.channels_num() != image2.channels_num())
        return false;

    size_t size = image1.size();
    const auto* buffer1 = image1.buffer();
    const auto* buffer2 = image2.buffer();
    for (size_t i = 0; i < size; ++i) {
        if (buffer1[i] != buffer2[i])
            return false;
    }

    return true;
}

bool images_same(const std::filesystem::path& image_path1, const std::filesystem::path& image_path2)
{
    MemoryImage image1(image_path1.string().c_str());
    MemoryImage image2(image_path2.string().c_str());

    return image1 == image2;
}

bool get_image_diff(const std::filesystem::path& image_path1, const std::filesystem::path& image_path2, const std::filesystem::path& image_diff)
{
    MemoryImage image1;
    MemoryImage image2;

    bool result;

    result = image1.load(image_path1.string().c_str());
    if (!result)
        return false;

    result = image2.load(image_path2.string().c_str());
    if (!result)
        return false;

    assert(image1);
    assert(image2);

    if (image1.size() != image2.size())
        return false;

    MemoryImage diff(image1.width(), image1.height(), image1.channels_num());

    size_t size = image1.size();

    const auto* buffer1 = image1.buffer();
    const auto* buffer2 = image2.buffer();
    auto* buffer3 = diff.buffer();

    assert(buffer1);
    assert(buffer2);
    assert(buffer3);

    result = true;
    for (size_t i = 0; i < size; ++i) {
        buffer3[i] = std::max(buffer1[i], buffer2[i]) - std::min(buffer1[i], buffer2[i]);
        result &= buffer3[i] != 0;
    }

    if (!diff.save(image_diff))
        throw std::runtime_error("get_image_diff: can't save diff image to file: " + image_diff.string());

    return result;
}
