#pragma once

#include <string>

namespace rprf
{

class Plugin
{
public:
	Plugin(const std::string_view& libraryName = "Tahoe64.dll");

	const std::string& name() const  { return m_name;     }
	int id() const                   { return m_pluginId; }

	Plugin(const Plugin&)            = delete;
	Plugin& operator=(const Plugin&) = delete;
private:
	const std::string m_name;
	int m_pluginId;
};

} // rpr
