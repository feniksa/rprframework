#include "ContextUtils.h"
#include "Plugin.h"
#include "Context.h"
#include "Error.h"
#include <iterator>
#include <string>
#include <vector>
#include <cassert>
#include <cstring>
#include <ranges>
#include <algorithm>
#include <sstream>

namespace
{
    const char* GenericGpuTranslate = "Generic GPU";

	static const std::vector<std::pair<int, int>> DevicesList
	{
		{ RPR_CREATION_FLAGS_ENABLE_CPU,  RPR_CONTEXT_CPU_NAME },
		{ RPR_CREATION_FLAGS_ENABLE_GPU0,  RPR_CONTEXT_GPU0_NAME },
		{ RPR_CREATION_FLAGS_ENABLE_GPU1,  RPR_CONTEXT_GPU1_NAME },
		{ RPR_CREATION_FLAGS_ENABLE_GPU2,  RPR_CONTEXT_GPU2_NAME },
		{ RPR_CREATION_FLAGS_ENABLE_GPU3,  RPR_CONTEXT_GPU3_NAME },
		{ RPR_CREATION_FLAGS_ENABLE_GPU4,  RPR_CONTEXT_GPU4_NAME },
		{ RPR_CREATION_FLAGS_ENABLE_GPU5,  RPR_CONTEXT_GPU5_NAME },
		{ RPR_CREATION_FLAGS_ENABLE_GPU6,  RPR_CONTEXT_GPU6_NAME },
		{ RPR_CREATION_FLAGS_ENABLE_GPU7,  RPR_CONTEXT_GPU7_NAME },
		{ RPR_CREATION_FLAGS_ENABLE_GPU8,  RPR_CONTEXT_GPU8_NAME },
		{ RPR_CREATION_FLAGS_ENABLE_GPU9,  RPR_CONTEXT_GPU9_NAME },
		{ RPR_CREATION_FLAGS_ENABLE_GPU10, RPR_CONTEXT_GPU10_NAME },
		{ RPR_CREATION_FLAGS_ENABLE_GPU11, RPR_CONTEXT_GPU11_NAME },
		{ RPR_CREATION_FLAGS_ENABLE_GPU12, RPR_CONTEXT_GPU12_NAME },
		{ RPR_CREATION_FLAGS_ENABLE_GPU13, RPR_CONTEXT_GPU13_NAME },
		{ RPR_CREATION_FLAGS_ENABLE_GPU14, RPR_CONTEXT_GPU14_NAME },
		{ RPR_CREATION_FLAGS_ENABLE_GPU15, RPR_CONTEXT_GPU15_NAME },
	};

	std::string GetDeviceIndexInRprNamespace(int rpr_gpu_index)
	{
		if (rpr_gpu_index == RPR_CREATION_FLAGS_ENABLE_CPU)
			return "CPU";

		const auto i = std::ranges::find(DevicesList.begin(), DevicesList.end(), rpr_gpu_index,
                                         &std::pair<int, int>::first);


		std::size_t index = std::distance(DevicesList.begin(), i) - 1;
		assert(index < DevicesList.size() - 1);

        return "GPU" + std::to_string(index);
	}
} // namespace

namespace rprf
{

gpu_list_t getAvailableDevices(const Plugin& plugin, const std::filesystem::path& cachePath, const std::filesystem::path& hipBin)
{
	gpu_list_t devices;

    for (size_t index = 0; index < DevicesList.size(); ++index) {
		try {
			Context testContext(plugin, cachePath, hipBin, DevicesList[index].first);

            if (plugin.type() == Plugin::Type::Northstar) {
                const std::string deviceName = testContext.getGpuName(index);
                devices.emplace_back(std::make_pair(DevicesList[index].first, deviceName));
            }
            else {
                devices.emplace_back(std::make_pair(DevicesList[index].first, GenericGpuTranslate));
            }
        }
        catch(...) {

		}
	} // end for

	return devices;
}

std::ostream& printAvailableDevices(const gpu_list_t& devices, std::ostream& stream)
{
	for (const auto&[gpu_index, gpu_name] : devices) {
		stream << GetDeviceIndexInRprNamespace(gpu_index) << ":\t" << gpu_name << "\n";
	}
	return stream;
}

int getContextCreationFlags(int gpu_number)
{
    switch(gpu_number) {
        case 0:
            return RPR_CREATION_FLAGS_ENABLE_GPU0;
        case 1:
            return RPR_CREATION_FLAGS_ENABLE_GPU1;
        case 2:
            return RPR_CREATION_FLAGS_ENABLE_GPU2;
        case 3:
            return RPR_CREATION_FLAGS_ENABLE_GPU3;
        case 4:
            return RPR_CREATION_FLAGS_ENABLE_GPU4;
        case 5:
            return RPR_CREATION_FLAGS_ENABLE_GPU5;
        case 6:
            return RPR_CREATION_FLAGS_ENABLE_GPU6;
        case 7:
            return RPR_CREATION_FLAGS_ENABLE_GPU7;
        case 8:
            return RPR_CREATION_FLAGS_ENABLE_GPU8;
        case 9:
            return RPR_CREATION_FLAGS_ENABLE_GPU9;
        case 10:
            return RPR_CREATION_FLAGS_ENABLE_GPU10;
        case 11:
            return RPR_CREATION_FLAGS_ENABLE_GPU11;
        case 12:
            return RPR_CREATION_FLAGS_ENABLE_GPU12;
        case 13:
            return RPR_CREATION_FLAGS_ENABLE_GPU13;
        case 14:
            return RPR_CREATION_FLAGS_ENABLE_GPU14;
        case 15:
            return RPR_CREATION_FLAGS_ENABLE_GPU15;
        default:
           throw std::runtime_error("getContextCreationFlags: bad gpu number");
    }
}

} // namespace
