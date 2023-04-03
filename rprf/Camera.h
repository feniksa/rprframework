#pragma once

#include "Context.h"
#include "CameraType.h"
#include "math/matrix.h"

namespace rprf
{

class Camera : public ContextObject<rpr_camera>
{
public:
	Camera(Context& context);

	void lookAt(float position_x, float position_y, float position_z, float aimedAt_x, float aimedAt_y, float aimedAt_z, float up_x, float up_y, float up_z);
	void setTransform(const rprf_math::matrix& transform, bool transpose);
	void setFocalLength(float length_mm = 35.0f);
	void setType(CameraType type);
	void setOrthoWidth(float width_m = 1);
	void setOrthoHeight(float height_m = 1);
	void setCameraNear(float near_m = 0.01f);
	void setCameraFar(float far_m = 100000000.0f);
	void setSensorWidthHeight(float width_mm = 36.0f, float height_mm = 24.0f);
	void setLensShift(float width_mm, float height_mm);

    rprf_math::matrix getTransform() const;
};

} // rpr
