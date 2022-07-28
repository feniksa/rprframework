#include "Shape.h"
#include "MaterialNode.h"
#include "Error.h"

namespace rprf
{

Shape::Shape(Context& context,
		rpr_float const * vertices, size_t num_vertices, rpr_int vertex_stride,
		rpr_float const * normals, size_t num_normals, rpr_int normal_stride,
		rpr_float const * texcoords, size_t num_texcoords, rpr_int texcoord_stride,
		rpr_int const * vertex_indices, rpr_int vidx_stride,
		rpr_int const * normal_indices, rpr_int nidx_stride,
		rpr_int const * texcoord_indices, rpr_int tidx_stride,
		rpr_int const * num_face_vertices, size_t num_faces,
		bool visibilty)
: m_context(context)
{
	int status;
	rpr_shape mesh;

	status = rprContextCreateMesh(context.instance(), vertices, num_vertices, vertex_stride,
		normals, num_normals, normal_stride,
		texcoords, num_texcoords, texcoord_stride,
		vertex_indices, vidx_stride,
		normal_indices, nidx_stride,
		texcoord_indices, tidx_stride,
		num_face_vertices, num_faces, &mesh);
	check(status);

	setInstance(std::move(mesh));

	setVisibility(visibilty);
}

Shape::Shape(const Shape& other)
: ContextObject<rpr_shape>(nullptr),
  m_context(other.m_context)
{
	int status;
	rpr_shape mesh;

	rpr_shape otherMesh = other.instance();

	status = rprContextCreateInstance(m_context.instance(), otherMesh, &mesh);
	check(status);

	setInstance(std::move(mesh));
}

void Shape::setTransform(const rprf_math::matrix& transform, bool transpose)
{
	int status;
	status = rprShapeSetTransform(*this, transpose, &transform.m00);
	check(status);
}

void Shape::setMaterial(const MaterialNode& node)
{
	int status;
	status = rprShapeSetMaterial(*this, node.instance());
	check(status);
}

void Shape::setVisibility(bool visibility)
{
	int status;
	status = rprShapeSetVisibility(*this, visibility);
	check(status);
}

} // namespace
