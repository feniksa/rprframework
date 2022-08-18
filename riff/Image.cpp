#include "Image.h"

namespace riff
{

Image::Image(rif_image&& image)
{
    setInstance(std::move(image));
}

} // namespace