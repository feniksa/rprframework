#include "LightEnvironment.h"
#include "Error.h"
#include "Image.h"

namespace rprf
{

LightEnvironment::LightEnvironment(Context& context)
{
	rpr_int status;
	rpr_light light;

	status = rprContextCreateEnvironmentLight(context.instance(), &light);
	check(status);

	setInstance(std::move(light));
}

void LightEnvironment::setImage(Image& image)
{
	int status;
	status = rprEnvironmentLightSetImage(*this, image.instance());
	check(status);
}

void LightEnvironment::setIntensityScale(float intensityScale)
{
	int status;
	status = rprEnvironmentLightSetIntensityScale(*this, intensityScale);
	check(status);
}

} // namespace
