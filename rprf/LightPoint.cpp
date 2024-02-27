#include "rprf/LightPoint.h"
#include "rprf/Error.h"

namespace rprf
{

LightPoint::LightPoint(Context& context)
{
	rpr_int status;
	rpr_light light;

	status = rprContextCreatePointLight(context.instance(), &light);
	check(status);

	setInstance(light);
}

void LightPoint::setRadianPower(float r, float g, float b)
{
	int status;

	status = rprPointLightSetRadiantPower3f(instance(), r, g, b);
	check(status);
}

} // namespace
