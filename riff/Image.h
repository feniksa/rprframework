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
    Image() = default;

    Image(Context& context, const std::filesystem::path& filePath);
    Image(Context& context, const ImageDescription& imageDescription, void* dataSource = nullptr);

    void create(Context& context, const ImageDescription& imageDescription, void* dataSource = nullptr);

    bool saveToFile(const std::filesystem::path& saveFilePath);

    ImageDescription getImageInfo() const;
private:

    void loadHDR(Context& context, const std::filesystem::path& filePath);
    void loadEXR(Context& context, const std::filesystem::path& filePath);
};

}


