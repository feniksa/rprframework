#pragma once

#include "ContextObject.h"
#include "ImageFilterType.h"
#include <vector>

namespace riff
{

class Context;
class Image;
class ContextQueue;

class ImageFilter : public ContextObject<rif_image_filter>
{
public:
    ImageFilter(Context& context, ImageFilterType type);
    ~ImageFilter() noexcept;
    void setParameter1u(const char* key, unsigned int parameter);
    void setParameterImage(const char* key, const Image& image);
    void setParameterImageArray(const char* key, std::vector<rif_image> & images);
    void setParameterFloatArray(const char* key, std::vector<float>& data);
    void setParameterString(const char* key, const char* value);

    void destroy() override;
private:
    friend ContextQueue;

    void registerQueue(ContextQueue* queue);
    void unregisterQueue(ContextQueue* queue);

    void setParameterImageArray(const char* key, rif_image* images, size_t size);
    void setParameterImageArray(const char* key, float* data, size_t size);

    std::vector<ContextQueue*> m_attachedToQueues;
};

} // namespace