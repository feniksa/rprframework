#pragma once

#include "rprf/Context.h"
#include "rprf/math/matrix.h"
#include "rprf/RadeonProRenderEnums.h"

namespace rprf
{

class MaterialNode;

class Shape : public ContextObject
{
public:
	RPRF_API
	Shape(Context& context, rpr_float const * vertices, size_t num_vertices, rpr_int vertex_stride,
		rpr_float const * normals, size_t num_normals, rpr_int normal_stride,
		rpr_float const * texcoords, size_t num_texcoords, rpr_int texcoord_stride,
		rpr_int const * vertex_indices, rpr_int vidx_stride,
		rpr_int const * normal_indices, rpr_int nidx_stride,
		rpr_int const * texcoord_indices, rpr_int tidx_stride,
		rpr_int const * num_face_vertices, size_t num_faces);

	RPRF_API
	Shape(const Shape& other);

	RPRF_API
	void setTransform(const rprf_math::matrix& transform, bool transpose);

	RPRF_API
	void setMaterial(const MaterialNode&);

	RPRF_API
	void setVisibility(bool visibility);

	RPRF_API
	void setVisibilityType(ShapeVisibilityType parameter, bool visible);
private:
	Context& m_context;
};

}
