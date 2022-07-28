#include "LightPoint.h"
#include "Error.h"

namespace rprf
{

LightPoint::LightPoint(Context& context)
{
	rpr_int status;
	rpr_light light;

	status = rprContextCreatePointLight(context.instance(), &light);
	check(status);

	setInstance(std::move(light));
}

void LightPoint::setRadianPower(float r, float g, float b)
{
	int status;

	status = rprPointLightSetRadiantPower3f(*this, r, g, b);
	check(status);
}

} // namespace
