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
    Image(Context& context, const std::filesystem::path& filePath);
    Image(Context& context, const ImageDescription& imageDescription, void* dataSource = nullptr);

    bool save(const std::filesystem::path& saveFilePath);

    ImageDescription getImageInfo() const;
private:
    void createRiffImage(Context& context, const ImageDescription& imageDescription, void* dataSource = nullptr);

    void loadHDR(Context& context, const std::filesystem::path& filePath);
    void loadEXR(Context& context, const std::filesystem::path& filePath);
};

}


