#include "Plugin.h"
#include <RadeonProRender.h>
#include <stdexcept>
#include <cassert>
#include <sstream>

namespace rprf
{

Plugin::Plugin(const std::string_view& libraryName)
: m_name(libraryName)
{
	assert(!libraryName.empty());

	m_pluginId = rprRegisterPlugin(libraryName.data());
	if (m_pluginId == -1) {
		std::stringstream s;
		s << "RadeonProRender error: Can't load library \"" << libraryName << "\"";

		throw std::runtime_error(s.str());
	}
}

Plugin::Plugin(Type type)
	: Plugin(GetDynamicLibraryName(type))
{
}


#ifdef _WIN32
const char* Plugin::GetDynamicLibraryName(Type type) 
{
	switch (type) 
	{
		case Type::Tahoe:
			return "Tahoe64.dll";
		case Type::Northstar:
			return "Northstar64.dll";
		case Type::Hybrid:
			return "Hybrid.dll";
		default:
			return "";
	}
	return "";
}
#else
const char* Plugin::GetDynamicLibraryName(Type type) 
{
	switch (type) 
	{
		case Type::Tahoe:
			return "libTahoe64.so";
		case Type::Northstar:
			return "libNorthstar64.so";
		case Type::Hybrid:
			return "libHybrid.so";
		default:
			return "";
	}
	return "";
}

#endif


} // rpr
