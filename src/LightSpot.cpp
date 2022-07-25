#include "LightSpot.h"
#include "Error.h"

namespace rprf
{

LightSpot::LightSpot(Context& context)
{
	rpr_int status;
	rpr_light light;

	status = rprContextCreateSpotLight(context.instance(), &light);
	check(status);

	setInstance(std::move(light));
}

void LightSpot::setRadianPower(float r, float g, float b)
{
	int status;

	status = rprSpotLightSetRadiantPower3f(*this, r, g, b);
	check(status);
}

void LightSpot::setConeShape(float iangle, float oangle)
{
	int status;
	status = rprSpotLightSetConeShape(*this, iangle, oangle);
	check(status);
}

} // namespace
