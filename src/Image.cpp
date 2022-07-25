#include "Image.h"
#include "Error.h"
#include <cassert>

namespace rprf
{

Image::Image(Context& context, const char* fileName)
{
	int status;
	rpr_image image;

	status = rprContextCreateImageFromFile(context.instance(), fileName, &image);
	check(status);

	setInstance(std::move(image));
}

Image::Image(Context& context, const int width, const int height, int numComponets, ComponentsType type, void* data)
{
	int status;
	rpr_image image;

	rpr_image_format format;

	assert(numComponets >= 3);
	assert(numComponets <= 4);

	format.num_components = numComponets;
	format.type = static_cast<int>(type);

	rpr_image_desc desc;
	desc.image_width  = width;
	desc.image_height = height;
	desc.image_depth = 1;
	desc.image_row_pitch = 0;
	desc.image_slice_pitch = 0;

	status = rprContextCreateImage(context.instance(), format, &desc, data, &image);
	check(status);

	setInstance(std::move(image));
}

} // namespace
