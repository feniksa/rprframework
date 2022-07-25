#pragma once

#include "Light.h"
//#include "Image.h"

namespace rprf
{

class LightEnvironment : public Light
{
public:
	LightEnvironment(Context& context);
	void setIntensityScale(float intensity);
	//void setImage(Image& image);
};

}
