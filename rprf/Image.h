#pragma once

#include "rprf/Context.h"
#include "rprf/ComponentType.h"

#include <filesystem>

namespace rprf
{
class Image : public ContextObject
{
public:
	RPRF_API
	Image(Context& context, const std::filesystem::path& fileName);

	RPRF_API
	Image(Context& context, int width, int height, int numComponets, ComponentsType type, void* data);
};

}
