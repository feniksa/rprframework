#include "rprf/LightSky.h"
#include "rprf/Error.h"

namespace rprf
{

LightSky::LightSky(Context& context)
{
	rpr_int status;
	rpr_light light;

	status = rprContextCreateSkyLight(context.instance(), &light);
	check(status);

	setInstance(light);
}

void LightSky::setTurbidity(float turbidity)
{
	rpr_int status;
	status = rprSkyLightSetTurbidity(instance(), turbidity);
	check(status);
}

void LightSky::setAlbedo(float albedo)
{
	rpr_int status;
	status = rprSkyLightSetAlbedo(instance(), albedo);
	check(status);
}

void LightSky::setDirection(float x, float y, float z)
{
	rpr_int status;
	status = rprSkyLightSetDirection(instance(), x, y, z);
	check(status);
}
void LightSky::setScale(float scale)
{
	rpr_int status;
	status = rprSkyLightSetScale(instance(), scale);
	check(status);
}


}
