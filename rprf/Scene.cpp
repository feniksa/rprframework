#include "rprf/Scene.h"
#include "rprf/Error.h"
#include "rprf/Shape.h"
#include "rprf/LightPoint.h"
#include "rprf/Camera.h"

namespace rprf
{

Scene::Scene(Context& context)
{
	rpr_int status;
	rpr_scene scene;
	status = rprContextCreateScene(context.instance(), &scene);
	check(status);

	setInstance(scene);
}

void Scene::attachShape(const Shape& shape)
{
	int status;
	status = rprSceneAttachShape(instance(), shape.instance());
	check(status);
}

void Scene::attachLight(const Light& light)
{
	int status;
	status = rprSceneAttachLight(instance(), light.instance());
	check(status);
}

void Scene::setCamera(const Camera& camera)
{
	int status;
	status = rprSceneSetCamera(instance(), camera.instance());
	check(status);
}

void Scene::detachShape(const Shape& light)
{
	int status;
	status = rprSceneDetachShape(instance(), light.instance());
	check(status);
}

void Scene::detachLight(const Light& light)
{
	int status;
	status = rprSceneDetachLight(instance(), light.instance());
	check(status);
}

}
