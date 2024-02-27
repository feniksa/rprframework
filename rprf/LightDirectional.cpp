#include "rprf/LightDirectional.h"
#include "rprf/Error.h"

namespace rprf
{

LightDirectional::LightDirectional(Context& context)
{
	rpr_int status;
	rpr_light light;

	status = rprContextCreateDirectionalLight(context.instance(), &light);
	check(status);

	setInstance(light);
}

void LightDirectional::setRadianPower(float r, float g, float b)
{
	int status;

	status = rprDirectionalLightSetRadiantPower3f(instance(), r, g, b);
	check(status);
}

void LightDirectional::setShadowSoftness(float coeff)
{
	int status;

	status = rprDirectionalLightSetShadowSoftnessAngle(instance(), coeff);
	check(status);
}

} // namespace
