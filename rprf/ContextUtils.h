#pragma once

#include <map>
#include <string>
#include <ostream>

namespace rprf
{
class Plugin;

using gpu_list_t = std::map<int, std::string>;

gpu_list_t getAvailableDevices(const Plugin& plugin, const char* cachePath);
std::ostream& printAvailableDevices(const gpu_list_t& devices, std::ostream& stream);

} // namespace
