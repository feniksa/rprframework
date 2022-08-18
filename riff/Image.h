#pragma once

#include "ContextObject.h"

namespace riff
{

class Image : public ContextObject<rif_image>
{
public:
    [[deprecated("Don't use this")]]
    Image(rif_image&& image);
};

}


