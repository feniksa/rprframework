#include "ContextUtils.h"
#include "Error.h"
#include <RadeonImageFilters.h>
#include <iostream>
#include <cassert>

namespace
{

constexpr std::size_t convertBytesToGigabytes(size_t bytes)
{
	return bytes / 1024 / 1024 / 1024;
}
}

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
	std::string deviceVendor;
#ifndef NDEBUG
	int deviceCount = getDeviceCount(backend);
	assert(deviceIndex < deviceCount);
#endif

	size_t dataSize;

	status = rifGetDeviceInfo(static_cast<int>(backend), deviceIndex, RIF_DEVICE_VENDOR, 0, nullptr, &dataSize);
	check(status);

	if (dataSize > 0)
		deviceVendor.resize(dataSize);
	else
		return deviceVendor;

	status = rifGetDeviceInfo(
			static_cast<int>(backend), deviceIndex, RIF_DEVICE_VENDOR,
			deviceVendor.size() * sizeof(char), deviceVendor.data(), &dataSize);
	check(status);

	if (deviceVendor.size() != dataSize)
		throw Error(RIF_ERROR_INTERNAL_ERROR, "getAvailableDevices: allocated buffer is not same size as required data");

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
	std::string deviceName;
#ifndef NDEBUG
	int deviceCount = getDeviceCount(backend);
	assert(deviceIndex < deviceCount);
#endif

	size_t dataSize;

	status = rifGetDeviceInfo(static_cast<int>(backend), deviceIndex, RIF_DEVICE_NAME, 0, nullptr, &dataSize);
	check(status);

	if (dataSize > 0)
		deviceName.resize(dataSize);
	else
		return deviceName;

	status = rifGetDeviceInfo(
			static_cast<int>(backend), deviceIndex, RIF_DEVICE_NAME,
			deviceName.size() * sizeof(char), deviceName.data(), &dataSize);
	check(status);

	if (deviceName.size() != dataSize)
		throw Error(RIF_ERROR_INTERNAL_ERROR, "getAvailableDevices: allocated buffer is not same size as required data");

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
	stream << gpuInfo.gpuName << "\n";
	stream << gpuInfo.vendorName << "\n";
	stream << convertBytesToGigabytes( gpuInfo.gpuMemorySizeInBytes) << " Gb";

	return stream;
}

std::ostream& printAvailableDevices(BackendType backend, std::ostream& stream)
{
	auto gpus = getAvailableDevices(backend);
	for(const auto& gpu : gpus)
	{
		stream << gpu << "\n";
	}


	return stream;
}

} // namespace
