#pragma once

#include "ContextObject.h"
#include "ImageDescription.h"
#include <filesystem>

namespace riff
{

class Context;

class Image : public ContextObject<rif_image>
{
public:
    Image(Context& context);

    Image(Context& context, const std::filesystem::path& filePath);
    Image(Context& context, const ImageDescription& imageDescription, void* dataSource = nullptr);

    Image copy();

    [[nodiscard]]
    bool load(const std::filesystem::path& filePath);
    void allocate(const ImageDescription& imageDescription, void* dataSource = nullptr);

    bool saveToFile(const std::filesystem::path& saveFilePath);

    ImageDescription getImageInfo() const;
private:
    bool loadHDR(const std::filesystem::path& filePath);
    bool loadEXR(const std::filesystem::path& filePath);
    bool loadPNG(const std::filesystem::path& filePath);
    bool loadJPG(const std::filesystem::path& filePath);

    Context& m_context;
};

}


