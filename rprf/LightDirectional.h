#pragma once

#include "rprf/Light.h"

namespace rprf
{

class LightDirectional : public Light
{
public:
	RPRF_API
	explicit LightDirectional(Context& context);

	RPRF_API
	void setRadianPower(float r, float g, float b);

	RPRF_API
	void setShadowSoftness(float coeff);
};

}
