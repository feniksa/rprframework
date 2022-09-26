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

Image::Image(Context& context)
: m_context(context)
{
}

Image::Image(Context& context, const std::filesystem::path& filePath)
: Image(context)
{
    if (!load(filePath))
        throw std::runtime_error("Can't load image: " + filePath.string());
}

Image::Image(Context& context, const ImageDescription& imageDescription, void* dataSource)
: Image(context)
{
    allocate(imageDescription, dataSource);
}

bool Image::saveToFile(const std::filesystem::path& saveFilePath)
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

bool Image::load(const std::filesystem::path& filePath)
{
    if (!std::filesystem::exists(filePath)) {
        return false;
    }

    std::filesystem::path extension = filePath.extension();

    if (extension == ".exr") {
        return loadEXR(filePath);
    } else if (extension == ".hdr" ) {
        return loadHDR(filePath);
    } else if (extension == ".png") {
        return loadPNG(filePath);
    } else {
        return false;
    }

    return true;
}

void Image::allocate(const ImageDescription& imageDescription, void* dataSource)
{
    rif_int status;
    rif_image image;

    status = rifContextCreateImage(m_context, &imageDescription, dataSource, &image);
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

bool Image::loadHDR(const std::filesystem::path& filePath)
{
    int width;
    int height;
    int num_components;
    float* rawData;

    rawData = stbi_loadf(filePath.string().c_str(), &width, &height, &num_components, 0);
    if (!rawData)
        return false;

    std::unique_ptr<float> rawDataPtr(rawData);

    const int arraySize = width * height * num_components;

    assert(arraySize > 0);
    assert(num_components);

    std::vector<float> data(arraySize);

    CopyAndConvert<float, float>(data.data(), rawDataPtr.get(), arraySize, 1.f);

    rif_image_desc desc;
    std::memset(&desc, 0, sizeof(desc));
    desc.type = RIF_COMPONENT_TYPE_FLOAT32;
    desc.image_width = width;
    desc.image_height = height;
    desc.num_components = num_components;

    allocate(desc, data.data());

    return true;
}


bool Image::loadJPG(const std::filesystem::path& filePath)
{
    ImageJpg colorImg;

    ReadJpg(path.c_str(), colorImg);
    width = colorImg.Width;
    height = colorImg.Height;
    num = colorImg.BPP;
    int arraySize = width * height;
    data = new float[arraySize * num];
    CopyAndConvert<float, float>((float *) data, &colorImg.Pixels[0], width * height * num, 1.f);
    desc.type = RIF_COMPONENT_TYPE_FLOAT32;
    return true;
}

bool Image::loadPNG(const std::filesystem::path& filePath)
{
    int width;
    int height;
    int num_components;
    float* rawData;
    rawData = reinterpret_cast<float *>(stbi_load(filePath.string().c_str(), &width, &height, &num_components, 0));
    if (!rawData) {
        return false;
    }

    std::unique_ptr<float> rawDataPtr(rawData);

    assert(num_components);

    rif_image_desc desc;
    std::memset(&desc, 0, sizeof(desc));
    desc.type = RIF_COMPONENT_TYPE_UINT8;
    desc.image_width = width;
    desc.image_height = height;
    desc.num_components = num_components;

    allocate(desc, rawDataPtr.get());

    return true;
}

bool Image::loadEXR(const std::filesystem::path& filePath)
{
    int width;
    int height;
    const char* err;
    float* rawRGBA;

    int ret = LoadEXR(&rawRGBA, &width, &height, filePath.string().c_str(), &err);
    if (ret != TINYEXR_SUCCESS) {
        throw std::runtime_error(err);
    }

    std::unique_ptr<float> rawDatraPtr(rawRGBA);

    const int num_components = 4;
    const int arraySize = width * height * num_components;

    assert(arraySize > 0);
    assert(num_components);

    std::vector<float> data(arraySize);

    CopyAndConvert<float, float>(data.data(), rawDatraPtr.get(), arraySize, 1.f);

    rif_image_desc desc;
    std::memset(&desc, 0, sizeof(desc));
    desc.type = RIF_COMPONENT_TYPE_FLOAT32;
    desc.image_width = width;
    desc.image_height = height;
    desc.num_components = num_components;

    allocate(desc, data.data());

    return true;
}


} // namespace
