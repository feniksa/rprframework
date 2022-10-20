#pragma once

#include <RadeonImageFilters.h>
#include <string>

namespace riff
{

enum class BackendType
{
	Openc = RIF_BACKEND_API_OPENCL,
	Metal = RIF_BACKEND_API_METAL,
	DirectX12 = RIF_BACKEND_API_DIRECTX12
};

std::string to_string(BackendType type);
std::pair<BackendType, bool> from_string(std::string type);

} // namespace
