#include "ImageFilter.h"
#include "Context.h"
#include "Image.h"

namespace riff
{

ImageFilter::ImageFilter(Context& context, ImageFilterType type)
{
    rif_int status;
    rif_image_filter filter;

    status = rifContextCreateImageFilter(context, static_cast<int>(type), &filter);
    check(status);

    setInstance(std::move(filter));
}

void ImageFilter::setParameter1u(const char* key, unsigned int parameter)
{
    rif_int status;

    status = rifImageFilterSetParameter1u(*this, key, parameter);
    check(status);
}

void ImageFilter::setParameterImage(const char* key, const Image& image)
{
    rif_int status;

    status = rifImageFilterSetParameterImage(*this, key, image);
    check(status);
}

void ImageFilter::setParameterImageArray(const char* key, std::vector<rif_image> & images)
{
    setParameterImageArray(key, &images[0], images.size());
}

void ImageFilter::setParameterFloatArray(const char* key, std::vector<float>& data)
{
    rif_int status;

    status = rifImageFilterSetParameterFloatArray(*this, key, data.data(), data.size());
    check(status);
}

void ImageFilter::setParameterImageArray(const char* key, rif_image* images, size_t size)
{
    rif_int status;

    status = rifImageFilterSetParameterImageArray(*this, key, images, size);
    check(status);
}

} // namespace riff
