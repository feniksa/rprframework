#include "Scene.h"
#include "Error.h"
#include "Shape.h"
#include "LightPoint.h"
#include "Camera.h"

namespace rprf
{

Scene::Scene(Context& context)
{
	rpr_int status;
	rpr_scene scene;
	status = rprContextCreateScene(context.instance(), &scene);
	check(status);

	setInstance(std::move(scene));
}

void Scene::attachShape(const Shape& shape)
{
	int status;
	status = rprSceneAttachShape(*this, shape.instance());
	check(status);
}

void Scene::attachLight(const Light& light)
{
	int status;
	status = rprSceneAttachLight(*this, light.instance());
	check(status);
}

void Scene::setCamera(const Camera& camera)
{
	int status;
	status = rprSceneSetCamera(*this, camera.instance());
	check(status);
}


}
