#include "Utils.h"
#include <stb/stb_image.h>

MemoryImage::MemoryImage(const char* path)
{
    if (!load(path))
        throw std::runtime_error("Can't load image: " + std::string(path));
}

bool MemoryImage::load(const char* path)
{
    stbi_uc* pixels = stbi_load(path, &width, &height, &channels_num, 4);
    if (!pixels) {
        return false;
    }
    buffer.reset(pixels);

    return true;
}

bool operator==(const MemoryImage& image1, const MemoryImage& image2)
{
    if (image1.width != image2.width)
        return false;
    if (image1.height != image2.height)
        return false;
    if (image1.channels_num != image2.channels_num)
        return false;

    if (!image1.buffer)
        return false;
    if (!image2.buffer)
        return false;

    size_t size = image1.width * image1.height * image1.channels_num;
    for (size_t i = 0; i < size; ++i) {
        const auto* buffer1 = image1.buffer.get();
        const auto* buffer2 = image2.buffer.get();

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