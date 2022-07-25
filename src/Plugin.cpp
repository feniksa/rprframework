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

} // rpr
