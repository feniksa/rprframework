#pragma once

#include <string>
#include <ostream>
#include <filesystem>
#include <vector>

namespace rprf
{
class Plugin;

using gpu_list_t = std::vector<std::pair<int, std::string>>;

gpu_list_t getAvailableDevices(const Plugin& plugin, const std::filesystem::path& cachePath, const std::filesystem::path& hipBin);
std::ostream& printAvailableDevices(const gpu_list_t& devices, std::ostream& stream);

int getContextCreationFlags(int gpu_number);

} // namespace
