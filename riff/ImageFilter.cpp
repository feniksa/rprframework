#include "ImageFilter.h"
#include "Context.h"
#include "Image.h"
#include "ContextQueue.h"

#include <algorithm>
#include <cassert>

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

ImageFilter::~ImageFilter() noexcept
{
    const char* errorMsg = "ImageFilter::~ImageFilter() error: ";
    try {
        destroy();
    } catch (const std::exception& e){
        std::cerr << errorMsg << e.what();
    }
    catch (...) {
        std::cerr << errorMsg << "Unknown error";
    }
}

void ImageFilter::destroy()
{
    for(ContextQueue* queue : m_attachedToQueues) {
        queue->detachFilter(this);
    }
    m_attachedToQueues.clear();

    ContextObject<rif_image_filter>::destroy();
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

void ImageFilter::setParameterString(const char* key, const char* value)
{
    rif_int status;

    status = rifImageFilterSetParameterString(*this, key, value);
    check(status);
}

void ImageFilter::registerQueue(ContextQueue* queue)
{
    auto it = std::find(m_attachedToQueues.begin(), m_attachedToQueues.end(), queue);
    if (it != m_attachedToQueues.end())
        return;

    m_attachedToQueues.push_back(queue);
}
void ImageFilter::unregisterQueue(ContextQueue* queue)
{
    auto it = std::find(m_attachedToQueues.begin(), m_attachedToQueues.end(), queue);
    if (it == m_attachedToQueues.end()) {
        throw std::runtime_error("ImageFilter: queue already unregistered");
    }

    ContextQueue* foundQueue = (*it);
    assert(foundQueue);

    m_attachedToQueues.erase(it);
}

} // namespace riff
