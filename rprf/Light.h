#pragma once

#include "rprf/Object.h"
#include "rprf/Context.h"
#include <math/matrix.h>

namespace rprf
{

class Light : public ContextObject
{
public:
	RPRF_API
	void setTransform(const rprf_math::matrix& transform, bool transpose);
};

} // namespace
