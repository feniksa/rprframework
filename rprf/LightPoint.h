#pragma once

#include "Light.h"

namespace rprf
{

class LightPoint : public Light
{
public:
	LightPoint(Context& context);

	void setRadianPower(float r, float g, float b);
};

}
