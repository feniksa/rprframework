#pragma once

#include "ContextObject.h"
#include <vector>

namespace riff
{

class Context;
class ImageFilter;
class Image;

class ContextQueue : public ContextObject<rif_command_queue>
{
public:
    ContextQueue(Context& context);
    ~ContextQueue() noexcept;

    void execute();

    void attachFilter(ImageFilter* filter, const Image* input, Image* output);
    void detachFilter(ImageFilter* filter);
    void detachAllFilters();
private:
    void detachFilter(rif_image_filter& filter);

    Context& m_context;
    std::vector<ImageFilter*> m_attachedFilters;
};

}
