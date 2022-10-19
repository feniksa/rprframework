#pragma once

#include <string>
#include <array>

namespace rprf
{

class Plugin
{
public:
	enum class Type { Tahoe, Northstar, Hybrid };

public:
	explicit Plugin(const std::string_view& libraryName);
	explicit Plugin(Type name);

	const std::string& name() const  { return m_name;     }
	int id() const                   { return m_pluginId; }

	Plugin(const Plugin&)            = delete;
	Plugin& operator=(const Plugin&) = delete;
private:
	static const char* GetDynamicLibraryName(Type type);

	const std::string m_name;
	int m_pluginId;
};

std::string to_string(const Plugin::Type type);
std::pair<Plugin::Type, bool> from_string(std::string type);

} // rpr
