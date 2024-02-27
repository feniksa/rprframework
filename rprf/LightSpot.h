#pragma once

#include "rprf/Light.h"

namespace rprf
{

class LightSpot : public Light
{
public:
	RPRF_API
	explicit LightSpot(Context& context);

	RPRF_API
	void setRadianPower(float r, float g, float b);

	RPRF_API
	void setConeShape(float iangle, float oangle);
};

}
