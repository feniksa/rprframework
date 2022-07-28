#include "Context.h"
#include "Plugin.h"
#include "Error.h"
#include "Scene.h"
#include "FrameBuffer.h"

#include <vector>

namespace rprf
{

Context::Context(const Plugin& plugin, const std::filesystem::path& cachePath, int createFlags)
: m_createFlags(createFlags)
{
	int status;

	std::vector<int> ids;
	ids.push_back(plugin.id());

	rpr_context context;
	status = rprCreateContext(RPR_API_VERSION, ids.data(), ids.size(), m_createFlags, NULL, cachePath.string().c_str(), &context);
	check(status);

	setInstance(std::move(context));

	status = rprContextSetActivePlugin(*this, ids.at(0));
	check(status);
}


void Context::setScene(const Scene& scene)
{
	int status;
	status = rprContextSetScene(*this, scene.instance());
	check(status);
}

void Context::setAOV(const FrameBuffer& frameBuffer)
{
	int status;
	status = rprContextSetAOV(*this, RPR_AOV_COLOR, frameBuffer.instance());
	check(status);
}

void Context::unsetAOV()
{
	int status;
	status = rprContextSetAOV(*this, RPR_AOV_COLOR, nullptr);
	check(status);
}

void Context::setParameter1u(unsigned int name, unsigned int value)
{
	int status;
	status = rprContextSetParameterByKey1u(*this, name, value);
	check(status);
}

void Context::setParameter1f(unsigned int name, float value)
{
	int status;
	status = rprContextSetParameterByKey1f(*this, name, value);
	check(status);
}

void Context::render()
{
	int status;
	status = rprContextRender(*this);
	check(status);
}


void Context::resolve(FrameBuffer* src, FrameBuffer* dst, bool noDisplayGamma)
{
	int status;
	status = rprContextResolveFrameBuffer(*this, src->instance(), dst->instance(), noDisplayGamma);
	check(status);
}

}
