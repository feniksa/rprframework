#pragma once

#include "Light.h"

namespace rprf
{

class Image;

class LightEnvironment : public Light
{
public:
	LightEnvironment(Context& context);
	void setIntensityScale(float intensity);
	void setImage(Image& image);
};

}
