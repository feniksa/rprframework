#pragma once

#include "Context.h"
#include "ComponentType.h"

namespace rprf
{
class Image : public ContextObject<rpr_image>
{
public:
	Image(Context& context, const char* fileName);
	Image(Context& context, const int width, const int height, int numComponets, ComponentsType type, void* data);
};

}
