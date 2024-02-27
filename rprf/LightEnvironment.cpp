#include "rprf/LightEnvironment.h"
#include "rprf/Error.h"
#include "rprf/Image.h"

namespace rprf
{

LightEnvironment::LightEnvironment(Context& context)
{
	rpr_int status;
	rpr_light light;

	status = rprContextCreateEnvironmentLight(context.instance(), &light);
	check(status);

	setInstance(light);
}

void LightEnvironment::setImage(Image& image)
{
	int status;
	status = rprEnvironmentLightSetImage(instance(), image.instance());
	check(status);
}

void LightEnvironment::setIntensityScale(float intensityScale)
{
	int status;
	status = rprEnvironmentLightSetIntensityScale(instance(), intensityScale);
	check(status);
}

} // namespace
