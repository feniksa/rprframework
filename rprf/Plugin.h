#pragma once

#include <string>

namespace rprf
{

class Plugin
{
public:
	enum class Type { Tahoe, Northstar, Hybrid};

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

} // rpr
