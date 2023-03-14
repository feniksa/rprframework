#include "Context.h"
#include "Plugin.h"
#include "Error.h"
#include "Scene.h"
#include "FrameBuffer.h"

#include <vector>

namespace
{
    int convertGpuName_index2rprflag(size_t gpuIndex)
    {
        static const std::array<int, 16> flagsTable
        {
            RPR_CONTEXT_GPU0_NAME,
            RPR_CONTEXT_GPU1_NAME,
            RPR_CONTEXT_GPU2_NAME,
            RPR_CONTEXT_GPU3_NAME,
            RPR_CONTEXT_GPU4_NAME,
            RPR_CONTEXT_GPU5_NAME,
            RPR_CONTEXT_GPU6_NAME,
            RPR_CONTEXT_GPU7_NAME,
            RPR_CONTEXT_GPU8_NAME,
            RPR_CONTEXT_GPU9_NAME,
            RPR_CONTEXT_GPU10_NAME,
            RPR_CONTEXT_GPU11_NAME,
            RPR_CONTEXT_GPU12_NAME,
            RPR_CONTEXT_GPU13_NAME,
            RPR_CONTEXT_GPU14_NAME,
            RPR_CONTEXT_GPU15_NAME,
        };

        return flagsTable.at(gpuIndex);
    }
}

namespace rprf
{

Context::Context(const Plugin& plugin, const std::filesystem::path& cachePath, int createFlags, unsigned int rprApiVersion)
: m_createFlags(createFlags)
{
	int status;

	std::vector<int> ids;
	ids.push_back(plugin.id());

	rpr_context context;
	status = rprCreateContext(rprApiVersion, ids.data(), ids.size(),
							  m_createFlags, NULL, cachePath.string().c_str(), &context);
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

void Context::setParameter1u(ContextInputType parameter, unsigned int value)
{
	int status;
	status = rprContextSetParameterByKey1u(*this, static_cast<int>(parameter), value);
	check(status);
}

void Context::setParameter1f(ContextInputType parameter, float value)
{
	int status;
	status = rprContextSetParameterByKey1f(*this, static_cast<int>(parameter), value);
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

std::string Context::getCpuName() const
{
    int status;

    size_t dataSize;
    status = rprContextGetInfo(*this, RPR_CONTEXT_CPU_NAME, 0, nullptr, &dataSize);
    check(status);

    if (dataSize == 0)
        return "";

    std::vector<char> buffer(dataSize);
    status = rprContextGetInfo(*this, RPR_CONTEXT_CPU_NAME,
            buffer.size(), buffer.data(), &dataSize);
    check(status);

    std::string deviceName;
    deviceName.reserve(dataSize - 1);

    std::copy(buffer.begin(), buffer.end() - 1, std::back_inserter(deviceName));

    return deviceName;
}
std::string Context::getGpuName(int gpuIndex) const
{
    int status;

    int rprFlag = convertGpuName_index2rprflag(gpuIndex);

    size_t dataSize;
    status = rprContextGetInfo(*this, rprFlag, 0, nullptr, &dataSize);
    check(status);

    if (dataSize == 0)
        return "";

    std::vector<char> buffer(dataSize);
    status = rprContextGetInfo(*this, rprFlag,
            buffer.size(), buffer.data(), &dataSize);
    check(status);

    std::string deviceName;
    deviceName.reserve(dataSize-1);
    std::copy(buffer.begin(), buffer.end() - 1, std::back_inserter(deviceName));

    return deviceName;
}

}
