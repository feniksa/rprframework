#pragma once
#include <RadeonProRender.h>

namespace tests
{

struct vertex
{
    rpr_float pos[3];
    rpr_float norm[3];
    rpr_float tex[2];
};

extern const vertex cube_data[];
extern const rpr_int indices[];
extern const rpr_int num_face_vertices[];
extern vertex plane_data[];

} // namespace
