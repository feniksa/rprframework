#pragma once

#include "Object.h"
#include "Context.h"
#include <math/matrix.h>

namespace rprf
{

class Light : public ContextObject<rpr_light>
{
public:
	void setTransform(const rprf_math::matrix& transform, bool transpose);
};

} // namespace
