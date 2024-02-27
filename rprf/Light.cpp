#include "rprf/Light.h"
#include "rprf/Error.h"

namespace rprf
{

void Light::setTransform(const rprf_math::matrix& transform, bool transpose)
{
	int status;
	status = rprLightSetTransform(instance(), transpose, &transform.m00);
	check(status);
}

} // namespace
