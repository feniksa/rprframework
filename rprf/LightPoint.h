#pragma once

#include "rprf/Light.h"

namespace rprf
{

class LightPoint : public Light
{
public:
	RPRF_API
	explicit LightPoint(Context& context);

	RPRF_API
	void setRadianPower(float r, float g, float b);
};

}
