#pragma once

#include "Context.h"
#include "math/matrix.h"
#include <vector>

namespace rprf
{

//class MaterialNode;

class Shape : public ContextObject<rpr_shape>
{
public:
	Shape(Context& context, rpr_float const * vertices, size_t num_vertices, rpr_int vertex_stride,
		rpr_float const * normals, size_t num_normals, rpr_int normal_stride,
		rpr_float const * texcoords, size_t num_texcoords, rpr_int texcoord_stride,
		rpr_int const * vertex_indices, rpr_int vidx_stride,
		rpr_int const * normal_indices, rpr_int nidx_stride,
		rpr_int const * texcoord_indices, rpr_int tidx_stride,
		rpr_int const * num_face_vertices, size_t num_faces,
		bool visibility = true);
	//Shape(const Shape& other)
	void setTransform(const rprf_math::matrix& transform, bool transpose);
	//void setMaterial(const MaterialNode&);
	void setVisibility(bool visibility);
private:
	Context& m_context;
};

}
