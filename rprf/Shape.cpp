#include "rprf/Shape.h"
#include "rprf/MaterialNode.h"
#include "rprf/Error.h"

namespace rprf
{

Shape::Shape(Context& context,
		rpr_float const * vertices, size_t num_vertices, rpr_int vertex_stride,
		rpr_float const * normals, size_t num_normals, rpr_int normal_stride,
		rpr_float const * texcoords, size_t num_texcoords, rpr_int texcoord_stride,
		rpr_int const * vertex_indices, rpr_int vidx_stride,
		rpr_int const * normal_indices, rpr_int nidx_stride,
		rpr_int const * texcoord_indices, rpr_int tidx_stride,
		rpr_int const * num_face_vertices, size_t num_faces)
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

	setInstance(mesh);
}

Shape::Shape(const Shape& other)
: ContextObject(nullptr),
  m_context(other.m_context)
{
	int status;
	rpr_shape mesh;

	rpr_shape otherMesh = other.instance();

	status = rprContextCreateInstance(m_context.instance(), otherMesh, &mesh);
	check(status);

	setInstance(mesh);
}

void Shape::setTransform(const rprf_math::matrix& transform, bool transpose)
{
	int status;
	status = rprShapeSetTransform(instance(), transpose, &transform.m00);
	check(status);
}

void Shape::setMaterial(const MaterialNode& node)
{
	int status;
	status = rprShapeSetMaterial(instance(), node.instance());
	check(status);
}

void Shape::setVisibility(bool visibility)
{
	int status;
	status = rprShapeSetVisibility(instance(), visibility);
	check(status);
}

void Shape::setVisibilityType(ShapeVisibilityType parameter, bool visible)
{
	int status;
	status = rprShapeSetVisibilityFlag(instance(), static_cast<unsigned int>(parameter), visible);
	check(status);
}

} // namespace
