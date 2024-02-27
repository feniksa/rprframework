#pragma once

#include "rprf/Context.h"

namespace rprf
{

class Shape;
class Camera;
class Light;

class Scene : public ContextObject
{
public:
	RPRF_API
	explicit Scene(Context& context);

	RPRF_API
	void attachShape(const Shape& shape);

	RPRF_API
	void attachLight(const Light& light);

	RPRF_API
	void setCamera(const Camera& camera);

	RPRF_API
	void detachShape(const Shape& shape);

	RPRF_API
	void detachLight(const Light& light);
};

}
