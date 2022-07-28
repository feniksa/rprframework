#pragma once

#include <RadeonProRender.h>
#include <cstdint>

namespace rprf
{
	enum class ComponentsType
	{
		Uint8   = RPR_COMPONENT_TYPE_UINT8,
		Uint32  = RPR_COMPONENT_TYPE_UINT32,
		Float16 = RPR_COMPONENT_TYPE_FLOAT16,
		Float32 = RPR_COMPONENT_TYPE_FLOAT32,
	};

	inline size_t sizeOf(ComponentsType type) // in bytes
	{
		switch (type) {
		case ComponentsType::Uint8:
			return 1;
		case ComponentsType::Uint32:
			return 4;
		case ComponentsType::Float16:
			return 2;
		case ComponentsType::Float32:
			return 4;
		default:
			return 0;
		}

		return 0;
	}
}
