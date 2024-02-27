#pragma once

#include "rprf/rprf.h"
#include "rprf/Context.h"
#include "rprf/CameraType.h"
#include "rprf/math/matrix.h"

namespace rprf
{

class Camera : public ContextObject
{
public:
	RPRF_API
	explicit Camera(Context& context);

	RPRF_API
	void lookAt(float position_x, float position_y, float position_z,
	            float aimedAt_x, float aimedAt_y, float aimedAt_z, 
	            float up_x, float up_y, float up_z);

	RPRF_API
	void setTransform(const rprf_math::matrix& transform, bool transpose);

	RPRF_API
	void setFocalLength(float length_mm = 35.0f);

	RPRF_API
	void setType(CameraType type);

	RPRF_API
	void setOrthoWidth(float width_m = 1);

	RPRF_API
	void setOrthoHeight(float height_m = 1);

	RPRF_API
	void setCameraNear(float near_m = 0.01f);

	RPRF_API
	void setCameraFar(float far_m = 100000000.0f);

	RPRF_API
	void setSensorWidthHeight(float width_mm = 36.0f, float height_mm = 24.0f);

	RPRF_API
	void setLensShift(float width_mm, float height_mm);

	[[nodiscard]]
	RPRF_API
	rprf_math::matrix getTransform() const;
};

} // namespace rprf
