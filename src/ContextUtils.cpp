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

		auto i = std::ranges::find(DevicesList.begin(), DevicesList.end(), rpr_gpu_index,
                            &std::pair<int, int>::first);


		std::size_t index = std::distance(DevicesList.begin(), i) - 1;
		assert(index < DevicesList.size() - 1);

        return "GPU" + std::to_string(index);
	}
} // namespace

namespace rprf
{

gpu_list_t getAvailableDevices(const Plugin& plugin, const char* cachePath)
{
	int status;

	std::map<int, std::string> devices;

	std::vector<int> ids;
	ids.push_back(plugin.id());


	for (size_t index = 0; index < DevicesList.size(); ++index) {
		rpr_context contextPurePointer;
		status = rprCreateContext(RPR_API_VERSION, ids.data(), ids.size(), DevicesList[index].first, nullptr, cachePath, &contextPurePointer);
		if (status != RPR_ERROR_UNSUPPORTED)
			check(status);
		else
			continue;

		ContextObject context(std::move(contextPurePointer));

		size_t dataSize;
		status = rprContextGetInfo(context, DevicesList[index].second, 0, nullptr, &dataSize);
		check(status);

		if (dataSize == 0)
			continue;

		std::string deviceName;
		deviceName.resize(dataSize);

		try {
			status = rprContextGetInfo(context, DevicesList[index].second,
					deviceName.size(), deviceName.data(), &dataSize);
			check(status);
		}
		catch(...)
		{
			continue;
		}

		assert(!deviceName.empty());

		devices[DevicesList[index].first] = deviceName;
	} // end for

	return devices;
}

std::ostream& printAvailableDevices(const gpu_list_t& devices, std::ostream& stream)
{
	stream << "Available GPU's for render:\n";

	for (const auto&[gpu_index, gpu_name] : devices) {
		stream << GetDeviceIndexInRprNamespace(gpu_index) << ":\t" << gpu_name << "\n";
	}
	return stream;
}

}
