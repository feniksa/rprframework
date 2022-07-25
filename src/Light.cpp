#include "Light.h"
#include "Error.h"

namespace rprf
{

void Light::setTransform(const rprf_math::matrix& transform, bool transpose)
{
	int status;
	status = rprLightSetTransform(*this, transpose, &transform.m00);
	check(status);
}

} // namespace
