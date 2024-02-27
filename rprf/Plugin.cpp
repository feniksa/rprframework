#include "rprf/Plugin.h"
#include <RadeonProRender.h>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <cstring>

namespace rprf
{

Plugin::Plugin(Type type)
: m_type(type)
{
	const char* libraryName = GetDynamicLibraryName(m_type);
	assert(strlen(libraryName) != 0);

	m_pluginId = rprRegisterPlugin(libraryName);
	if (m_pluginId == -1) {
		std::stringstream s;
		s << "RadeonProRender error: Can't load library \"" << libraryName << "\"";

		throw std::runtime_error(s.str());
	}
}

#ifdef _WIN32
const char* Plugin::GetDynamicLibraryName(Type type) noexcept
{
	switch (type)
	{
		case Type::Tahoe:
			return "Tahoe64.dll";
		case Type::Northstar:
			return "Northstar64.dll";
		case Type::Hybrid:
			return "Hybrid.dll";
		case Type::HybridPro:
			return "HybridPro.dll";
		default:
			return "";
	}
	return "";
}
#else
const char* Plugin::GetDynamicLibraryName(Type type) noexcept
{
	switch (type)
	{
		case Type::Tahoe:
			return "libTahoe64.so";
		case Type::Northstar:
			return "libNorthstar64.so";
		case Type::Hybrid:
			return "Hybrid.so";
		case Type::HybridPro:
			return "HybridPro.so";
		default:
			return "";
	}
	return "";
}
#endif

// tools

std::string to_string(Plugin::Type type)
{
	switch(type)
	{
		case Plugin::Type::Hybrid:
			return "Hybrid";
		case Plugin::Type::HybridPro:
			return "HybridPro";
		case Plugin::Type::Tahoe:
			return "Tahoe";
		case Plugin::Type::Northstar:
			return "Northstar";
		default:
			throw std::runtime_error("rprf::Plugin::to_string(const Plugin::Type) is not know");
	}
}

std::pair<Plugin::Type, bool> from_string(std::string type)
{
	std::transform(type.begin(), type.end(), type.begin(),
				   [](unsigned char c){ return std::tolower(c); });

	if (type == "hybrid")
		return {Plugin::Type::Hybrid, true};
	if (type == "hybridpro")
		return {Plugin::Type::HybridPro, true};
	if (type == "northstar")
		return {Plugin::Type::Northstar, true};
	if (type == "tahoe")
		return {Plugin::Type::Tahoe, true};

	return {Plugin::Type::Northstar, false};
}

} // rpr
