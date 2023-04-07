#include "ContextUtils.h"
#include "Error.h"

#include <RadeonImageFilters.h>

#include <iomanip>
#include <iostream>
#include <cassert>

namespace
{

constexpr float convertBytesToGigabytes(size_t bytes)
{
	return static_cast<float>(bytes) / 1024.0f / 1024.0f / 1024.0f;
}

} // namespace

namespace riff
{

std::size_t getDeviceMemorySize(BackendType backend, int deviceIndex)
{
	rif_int status;
	std::size_t memorySize;
#ifndef NDEBUG
	int deviceCount = getDeviceCount(backend);
	assert(deviceIndex < deviceCount);
#endif

	size_t dataSize;
	status = rifGetDeviceInfo(static_cast<int>(backend), deviceIndex, RIF_DEVICE_MEMORY_SIZE, 0, nullptr, &dataSize);
	check(status);

	static_assert(sizeof(memorySize) == 8);

	if (dataSize != sizeof(memorySize))
		throw Error(RIF_ERROR_INTERNAL_ERROR, "getDeviceMemorySize: data struct is not 8 bytes long");

	status = rifGetDeviceInfo(
			static_cast<int>(backend), deviceIndex, RIF_DEVICE_MEMORY_SIZE,
			sizeof(memorySize), &memorySize, &dataSize);
	check(status);

	if (sizeof(memorySize) != dataSize)
		throw Error(RIF_ERROR_INTERNAL_ERROR, "getDeviceMemorySize: function return non-expected datastruct size");

	return memorySize;
}

std::string getDeviceVendor(BackendType backend, int deviceIndex)
{
	rif_int status;
#ifndef NDEBUG
	int deviceCount = getDeviceCount(backend);
	assert(deviceIndex < deviceCount);
#endif

	size_t dataSize;

	status = rifGetDeviceInfo(static_cast<int>(backend), deviceIndex, RIF_DEVICE_VENDOR, 0, nullptr, &dataSize);
	check(status);

	if (dataSize == 0)
        return "";

    std::vector<char> buffer(dataSize);

	status = rifGetDeviceInfo(
			static_cast<int>(backend), deviceIndex, RIF_DEVICE_VENDOR,
			buffer.size() * sizeof(char), buffer.data(), &dataSize);
	check(status);

	if (buffer.size() != dataSize || dataSize == 0)
		throw Error(RIF_ERROR_INTERNAL_ERROR, "getAvailableDevices: allocated buffer is not same size as required data");

    std::string deviceVendor;
    deviceVendor.reserve(buffer.size() - 1);
    std::copy(buffer.begin(), buffer.end() - 1, std::back_inserter(deviceVendor));

	return deviceVendor;
}

gpu_list_t getAvailableDevices(BackendType backend)
{
	gpu_list_t gpus;

	int deviceCount = getDeviceCount(backend);
	gpus.resize(deviceCount);

	for (int deviceIndex = 0; deviceIndex < deviceCount; ++deviceIndex) {
		gpus[deviceIndex] =
		{
			.deviceId             = deviceIndex,
			.gpuName              = getDeviceName(backend, deviceIndex),
			.vendorName           = getDeviceVendor(backend, deviceIndex),
			.gpuMemorySizeInBytes = getDeviceMemorySize(backend, deviceIndex)
		};
	}

	return gpus;
}

std::string getDeviceName(BackendType backend, int deviceIndex)
{
	rif_int status;
#ifndef NDEBUG
	int deviceCount = getDeviceCount(backend);
	assert(deviceIndex < deviceCount);
#endif

	size_t dataSize;

	status = rifGetDeviceInfo(static_cast<int>(backend), deviceIndex,
                              RIF_DEVICE_NAME, 0, nullptr, &dataSize);
	check(status);

	if (dataSize == 0)
        return "";

    std::vector<char> buffer(dataSize);

	status = rifGetDeviceInfo(
			static_cast<int>(backend), deviceIndex, RIF_DEVICE_NAME,
			buffer.size() * sizeof(char), buffer.data(), &dataSize);
	check(status);

	if (buffer.size() != dataSize || dataSize == 0)
		throw Error(RIF_ERROR_INTERNAL_ERROR,
                    "getAvailableDevices: allocated buffer is not same size as required data");

    std::string deviceName;
    deviceName.reserve(buffer.size() - 1);
    std::copy(buffer.begin(), buffer.end() - 1, std::back_inserter(deviceName));

	return deviceName;
}

int getDeviceCount(BackendType backend)
{
	rif_int status;
	int deviceCount = 0;

	status = rifGetDeviceCount(static_cast<int>(backend), &deviceCount);
	check(status);

	return deviceCount;;
}

std::ostream& operator<<(std::ostream& stream, const GpuInfo& gpuInfo)
{
	stream << gpuInfo.gpuName << " ";

	auto flags = stream.flags();
	stream << std::fixed << std::setprecision(2) << convertBytesToGigabytes( gpuInfo.gpuMemorySizeInBytes) << " Gb";
	stream.flags(flags);

	return stream;
}

std::ostream& printAvailableDevices(const gpu_list_t& gpus, std::ostream& stream)
{
	for(const auto& gpu : gpus) {
		stream << gpu << "\n";
	}
	return stream;
}

} // namespace
