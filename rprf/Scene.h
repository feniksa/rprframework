#pragma once

#include "Context.h"

namespace rprf
{

class Shape;
class Camera;
class Light;

class Scene : public ContextObject<rpr_scene>
{
public:
	Scene(Context& context);

	void attachShape(const Shape& shape);
	void attachLight(const Light& light);
	void setCamera(const Camera& camera);


	void detachShape(const Shape& shape);
	void detachLight(const Light& light);
};

}
