#pragma once

#include <string>

namespace rprf
{

class Plugin
{
public:
	enum class Type { Tahoe, Northstar, Hybrid, HybridPro };

public:
	explicit Plugin(Type name);

	const char* name() const noexcept         { return GetDynamicLibraryName(m_type); }
	int id() const noexcept                   { return m_pluginId; }
	Type type() const noexcept                { return m_type;     }

	Plugin(const Plugin&)            = delete;
	Plugin& operator=(const Plugin&) = delete;
protected:
	explicit Plugin(const std::string_view& libraryName);
private:
	static const char* GetDynamicLibraryName(Type type) noexcept;

	Type m_type;
	int m_pluginId;
};

std::string to_string(const Plugin::Type type);
std::pair<Plugin::Type, bool> from_string(std::string type);

} // rpr
