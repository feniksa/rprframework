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

    void attachFilter(const ImageFilter& filter, const Image& input, Image* output);
    void detachFilter(const ImageFilter& filter);
    void detachAllFilters(bool ignoreErrors = true);
private:
    Context& m_context;
    std::vector<const ImageFilter*> m_attachedFilters;
};

}
