#pragma once

#include "rprf/Light.h"

namespace rprf
{

class LightSky : public Light
{
public:
	explicit LightSky(Context& context);

	void setTurbidity(float turbidity);
	void setAlbedo(float albedo);
	void setDirection(float x, float y, float z);
	void setScale(float scale);
};

}
