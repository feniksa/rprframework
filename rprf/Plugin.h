#pragma once

#include "rprf/rprf.h"
#include <string>

namespace rprf
{

class Plugin
{
public:
	enum class Type { Tahoe, Northstar, Hybrid, HybridPro };

public:
	RPRF_API
	explicit Plugin(Type name);

	[[nodiscard]]
	RPRF_API
	const char* name() const noexcept         { return GetDynamicLibraryName(m_type); }

	[[nodiscard]]
	RPRF_API
	int id() const noexcept                   { return m_pluginId; }

	[[nodiscard]]
	RPRF_API
	Type type() const noexcept                { return m_type;     }

	Plugin(const Plugin&)            = delete;
	Plugin& operator=(const Plugin&) = delete;
protected:
	//RPRF_API
	//explicit Plugin(const std::string_view& libraryName);
private:
	static const char* GetDynamicLibraryName(Type type) noexcept;

	Type m_type;
	int m_pluginId;
};

RPRF_API
std::string to_string(Plugin::Type type);

RPRF_API
std::pair<Plugin::Type, bool> from_string(std::string type);

} // rpr
