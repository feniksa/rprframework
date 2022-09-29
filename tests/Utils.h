#pragma once

#include <memory>
#include <exception>
#include <filesystem>

struct MemoryImage
{
    int width;
    int height;
    int channels_num;
    std::unique_ptr<unsigned char> buffer;

    MemoryImage() = default;
    MemoryImage(MemoryImage&&) = default;

    explicit MemoryImage(const char* path);
    bool load(const char* path);
    bool save(const char* path) const;
};

bool operator==(const MemoryImage& image1, const MemoryImage& image2);
bool images_same(const std::filesystem::path& image_path1, const std::filesystem::path& image_path2);
