set (HEADERS
	eLut.h
	float2.h
	float3.h
	half.h
	int2.h
	mathutils.h
	matrix.h
	quaternion.h
	toFloat.h
)

set(SOURCES
	half.cpp
)

add_library(rprf_math STATIC ${SOURCES} ${HEADERS})
