#pragma once

#include "Light.h"

namespace rprf
{

class LightDirectional : public Light
{
public:
	LightDirectional(Context& context);
	void setRadianPower(float r, float g, float b);
	void setShadowSoftness(float coeff);
};

}
