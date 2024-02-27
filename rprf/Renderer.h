#pragma once

#include "rprf/Context.h"

namespace rprf
{

class Renderer
{
public:
	RPRF_API
	Renderer();

	RPRF_API
	void start();

	RPRF_API
	void stop();
};

} // namespace
