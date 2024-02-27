#include "rprf/Camera.h"
#include "rprf/Error.h"
#include <cassert>

namespace rprf
{

Camera::Camera(Context& context)
{
	rpr_int status;
	rpr_camera camera;

	status = rprContextCreateCamera(context.instance(), &camera);
	check(status);

	setInstance(camera);
}

void Camera::lookAt(float position_x, float position_y, float position_z, float aimedAt_x, float aimedAt_y, float aimedAt_z, float up_x, float up_y, float up_z)
{
	rpr_int status;
	status = rprCameraLookAt(instance(), position_x, position_y, position_z, aimedAt_x, aimedAt_y, aimedAt_z, up_x, up_y, up_z);
	check(status);
}

void Camera::setTransform(const rprf_math::matrix& transform, bool transpose)
{
	rpr_int status;
	status = rprCameraSetTransform(instance(), transpose, &transform.m00);
	check(status);
}

void Camera::setFocalLength(float lenght)
{
	rpr_int status;
	status = rprCameraSetFocalLength(instance(), lenght);
	check(status);
}

void Camera::setType(CameraType type)
{
	rpr_int status;
	status = rprCameraSetMode(instance(), static_cast<int>(type));
	check(status);
}

void Camera::setOrthoWidth(float width)
{
	rpr_int status;
	status = rprCameraSetOrthoWidth(instance(), width);
	check(status);
}

void Camera::setOrthoHeight(float height)
{
	rpr_int status;
	status = rprCameraSetOrthoHeight(instance(), height);
	check(status);
}

void Camera::setCameraNear(float near)
{
	rpr_int status;
	status = rprCameraSetNearPlane(instance(), near);
	check(status);
}

void Camera::setCameraFar(float far)
{
	rpr_int status;
	status = rprCameraSetFarPlane(instance(), far);
	check(status);
}

void Camera::setSensorWidthHeight(float width_mm, float height_mm)
{
	rpr_int status;
	status = rprCameraSetSensorSize(instance(), width_mm, height_mm);
	check(status);
}

void Camera::setLensShift(float width_mm, float height_mm)
{
	rpr_int status;
	status = rprCameraSetLensShift(instance(), width_mm, height_mm);
	check(status);
}

rprf_math::matrix Camera::getTransform() const
{
    rprf_math::matrix m;

    rpr_int status;

    rpr_camera_info info = RPR_CAMERA_TRANSFORM;

    size_t returnSize;

    status = rprCameraGetInfo(instance(), info, sizeof(m.m), &m.m, &returnSize);
    check(status);

    assert(returnSize == sizeof(m.m));

    return m;
}

}
