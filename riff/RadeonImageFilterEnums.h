#pragma once
#include <RadeonImageFilters.h>

namespace riff
{

enum class BackendType
{
	Openc = RIF_BACKEND_API_OPENCL,
	Metal = RIF_BACKEND_API_METAL,
	DirectX12 = RIF_BACKEND_API_DIRECTX12
};

} // namespace
