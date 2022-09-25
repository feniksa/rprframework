#include "Image.h"
#include "Context.h"
#include "ReadMemMapped.h"

#include <tinyexr/tinyexr.h>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <cassert>
#include <functional>
#include <cstring>

#define stbi_check_r(s) if (s == 0) return false;

namespace
{
    template<typename dstT, typename srcT>
    void CopyAndConvert(dstT *dst, srcT *src, size_t size, dstT factor, bool clamp = false) {
        for (size_t i = 0; i < size; ++i) {
            float val = static_cast<float>(src[i] * factor);
            if (clamp) {
                val = (val > factor) ? factor : val;
                val = (val < 0) ? 0 : val;
            }
            dst[i] = static_cast<dstT>(val);
        }
    }
}

namespace riff
{

void Image::loadHDR(Context&, const std::filesystem::path&)
{
    throw std::runtime_error("not implemented");
}

void Image::loadEXR(Context& context, const std::filesystem::path& filePath)
{
    int width;
    int height;
    const char* err;
    float* rawRGBA;

    if (!std::filesystem::exists(filePath)) {
        throw std::runtime_error("riff::Image::loadEXR: can't load file " + filePath.string());
    }

    int ret = LoadEXR(&rawRGBA, &width, &height, filePath.string().c_str(), &err);
    if (ret != TINYEXR_SUCCESS) {
        throw std::runtime_error(err);
    }

    const int num_components = 4;
    const int arraySize = width * height * num_components;

    assert(arraySize > 0);
    assert(num_components);

    std::vector<float> data(arraySize);

    CopyAndConvert<float, float>(data.data(), rawRGBA, arraySize, 1.f);

    rif_image_desc desc;
	std::memset(&desc, 0, sizeof(desc));
    desc.type = RIF_COMPONENT_TYPE_FLOAT32;
    desc.image_width = width;
    desc.image_height = height;
    desc.num_components = num_components;

    createRiffImage(context, desc, data.data());
}

Image::Image(Context& context, const std::filesystem::path& filePath)
{
    std::filesystem::path extension = filePath.extension();

    if (extension == ".exr") {
        loadEXR(context, filePath);
    } else if (extension == ".hdr" ) {
        loadHDR(context, filePath);
    } else {
        throw std::runtime_error("Unsupported file");
    }
}

Image::Image(Context& context, const ImageDescription& imageDescription, void* dataSource)
{
    createRiffImage(context, imageDescription, dataSource);
}

bool Image::save(const std::filesystem::path& saveFilePath)
{
    if (!*this) {
        return false;
    }

    std::filesystem::path ext = saveFilePath.extension();

    ImageDescription desc = getImageInfo();
    ReadMemMapped mappedForReadMemory(*this);

    int status;
    if (ext == ".png") {
        status = stbi_write_png(saveFilePath.string().c_str(), desc.image_width, desc.image_height,
                                desc.num_components, mappedForReadMemory.data(), 0);
        stbi_check_r(status);
    } else if (ext == ".jpg" ) {
        status = stbi_write_jpg(saveFilePath.string().c_str(), desc.image_width, desc.image_height,
                                desc.num_components, mappedForReadMemory.data(), 0);
        stbi_check_r(status);
    } else if (ext == ".bmp") {
        status = stbi_write_bmp(saveFilePath.string().c_str(), desc.image_width, desc.image_height,
                                desc.num_components, mappedForReadMemory.data());
        stbi_check_r(status);
    } else if (ext == ".tga" ) {
        status = stbi_write_tga(saveFilePath.string().c_str(), desc.image_width, desc.image_height,
                                desc.num_components, mappedForReadMemory.data());
        stbi_check_r(status);
    } else
        return false;

    return true;
}

void Image::createRiffImage(Context& context, const ImageDescription& imageDescription, void* dataSource)
{
    rif_int status;
    rif_image image;

    status = rifContextCreateImage(context, &imageDescription, dataSource, &image);
    check(status);

    setInstance(std::move(image));
}

ImageDescription Image::getImageInfo() const
{
    rif_image_desc desc;
    size_t retSize;
    rif_int status;

    status = rifImageGetInfo(*this, RIF_IMAGE_DESC, sizeof(desc), &desc, &retSize);
    check(status);

    if (retSize != sizeof(desc))
        throw std::logic_error("Image::getImageInfo(): riff returned not enough bytes. API internal error detected");

    return desc;
}

} // namespace
