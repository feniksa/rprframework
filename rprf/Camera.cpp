#include "Camera.h"
#include "Error.h"

namespace rprf
{
Camera::Camera(Context& context)
{
	rpr_int status;
	rpr_camera camera;

	status = rprContextCreateCamera(context.instance(), &camera);
	check(status);

	setInstance(std::move(camera));
}

void Camera::lookAt(float position_x, float position_y, float position_z, float aimedAt_x, float aimedAt_y, float aimedAt_z, float up_x, float up_y, float up_z)
{
	rpr_int status;
	status = rprCameraLookAt(*this, position_x, position_y, position_z, aimedAt_x, aimedAt_y, aimedAt_z, up_x, up_y, up_z);
	check(status);
}

void Camera::setTransform(const rprf_math::matrix& _transform, bool transpose)
{
	m_transform = _transform;

	rpr_int status;
	status = rprCameraSetTransform(*this, transpose, &m_transform.m00);
	check(status);
}

void Camera::setFocalLength(float lenght)
{
	rpr_int status;
	status = rprCameraSetFocalLength(*this, lenght);
	check(status);
}

void Camera::setType(CameraType type)
{
	rpr_int status;
	status = rprCameraSetMode(*this, static_cast<int>(type));
	check(status);
}

void Camera::setOrthoWidth(float width)
{
	rpr_int status;
	status = rprCameraSetOrthoWidth(*this, width);
	check(status);
}

void Camera::setOrthoHeight(float height)
{
	rpr_int status;
	status = rprCameraSetOrthoHeight(*this, height);
	check(status);
}

void Camera::setCameraNear(float near)
{
	rpr_int status;
	status = rprCameraSetNearPlane(*this, near);
	check(status);
}

void Camera::setCameraFar(float far)
{
	rpr_int status;
	status = rprCameraSetFarPlane(*this, far);
	check(status);
}

void Camera::setSensorWidthHeight(float width_mm, float height_mm)
{
	rpr_int status;
	status = rprCameraSetSensorSize(*this, width_mm, height_mm);
	check(status);
}

void Camera::setLensShift(float width_mm, float height_mm)
{
	rpr_int status;
	status = rprCameraSetLensShift(*this, width_mm, height_mm);
	check(status);
}

}
