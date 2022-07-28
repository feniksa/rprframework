#pragma once

#include "Light.h"

namespace rprf
{

class LightSpot : public Light
{
public:
	LightSpot(Context& context);

	void setRadianPower(float r, float g, float b);
	void setConeShape(float iangle, float oangle);
};

}
