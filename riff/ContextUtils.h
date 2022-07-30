#pragma once

#include "RadeonImageFilterEnums.h"
#include <vector>
#include <string>

namespace riff
{

struct GpuInfo
{
	int deviceId;
	std::string gpuName;
	std::string vendorName;
	std::size_t gpuMemorySizeInBytes;
};

using gpu_list_t = std::vector<GpuInfo>;

int getDeviceCount(BackendType backend);
gpu_list_t getAvailableDevices(BackendType backend);

std::string getDeviceVendor(BackendType backend, int deviceIndex);
std::size_t getDeviceMemorySize(BackendType backend, int deviceIndex);
std::string getDeviceName(BackendType backend, int deviceIndex);

std::ostream& printAvailableDevices(const gpu_list_t& gpus, std::ostream& stream);
std::ostream& operator<<(std::ostream& stream, const GpuInfo& gpuInfo);

} // namespace
