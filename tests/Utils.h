#pragma once

#include <memory>
#include <exception>
#include <filesystem>

class MemoryImage
{
public:
    MemoryImage() = default;
    MemoryImage(MemoryImage&&) = default;

    explicit MemoryImage(const std::filesystem::path& path);
    explicit MemoryImage(int width, int height, int channels_num);

    bool load(const std::filesystem::path& path);
    bool save(const std::filesystem::path& path) const;
    void clear();

    int width() const noexcept { return m_width; }
    int height() const noexcept { return m_height; }
    int channels_num() const noexcept { return m_channels_num; }
    int size() const noexcept { return m_width * m_height * m_channels_num; }

    unsigned char& operator[] (size_t index) { return m_buffer.get()[index];}
    const unsigned char& operator[] (size_t index) const { return m_buffer.get()[index]; }

    const unsigned char* buffer() const { return m_buffer.get(); }
    unsigned char* buffer() { return m_buffer.get(); }

    operator bool() const { return m_width > 0 && m_height > 0 && m_channels_num > 0 && m_buffer; }

private:
    int m_width;
    int m_height;
    int m_channels_num;
    std::unique_ptr<unsigned char> m_buffer;

};

bool operator==(const MemoryImage& image1, const MemoryImage& image2);
bool images_same(const std::filesystem::path& image_path1, const std::filesystem::path& image_path2);
bool get_image_diff(const std::filesystem::path& image_path1, const std::filesystem::path& image_path2, const std::filesystem::path& image_diff);
