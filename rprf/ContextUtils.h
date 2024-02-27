#pragma once

#include "rprf/rprf.h"

#include <string>
#include <ostream>
#include <filesystem>
#include <vector>

namespace rprf
{
class Plugin;
using gpu_list_t = std::vector<std::pair<int, std::string>>;

RPRF_API
gpu_list_t getAvailableDevices(const Plugin& plugin, const std::filesystem::path& cachePath, const std::filesystem::path& hipBin);

RPRF_API
std::ostream& printAvailableDevices(const gpu_list_t& devices, std::ostream& stream);

RPRF_API
int getContextCreationFlags(int gpu_number);

} // namespace
