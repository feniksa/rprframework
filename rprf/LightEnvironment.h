#pragma once

#include "rprf/Light.h"

namespace rprf
{

class Image;

class LightEnvironment : public Light
{
public:
	RPRF_API
	explicit LightEnvironment(Context& context);

	RPRF_API
	void setIntensityScale(float intensity);

	RPRF_API
	void setImage(Image& image);
};

}
