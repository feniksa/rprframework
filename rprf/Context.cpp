#include "rprf/Context.h"
#include "rprf/Plugin.h"
#include "rprf/Error.h"
#include "rprf/Scene.h"
#include "rprf/FrameBuffer.h"

#include <array>
#include <vector>
#include <sstream>

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

std::ostream& operator<< (std::ostream& stream, const std::vector<std::filesystem::path>& dirs)
{
    for (const auto& d : dirs) {
	    stream << d << ", ";
    }
	return stream;
}

} // namespace

namespace rprf
{

Context::Context(const Plugin& plugin, const std::filesystem::path& cachePath, const std::filesystem::path& hipKernelsPath,
                 int createFlags, unsigned int rprApiVersion)
{
	int status;

    std::vector<rpr_context_properties> contextProperties;
    std::string hipPath;

    if (plugin.type() == Plugin::Type::Northstar) {
        hipPath = processHipKernelsPath(hipKernelsPath).string();
        contextProperties.push_back(reinterpret_cast<rpr_context_properties>(RPR_CONTEXT_PRECOMPILED_BINARY_PATH));
        contextProperties.push_back(reinterpret_cast<rpr_context_properties>(hipPath.data()));
    }
    contextProperties.push_back(0);

	std::vector<int> ids;
	ids.push_back(plugin.id());

	rpr_context context;
	status = rprCreateContext(rprApiVersion, ids.data(), ids.size(),
							  createFlags, contextProperties.data(), cachePath.string().c_str(), &context);
	check(status);

	setInstance(context);

	status = rprContextSetActivePlugin(instance(), ids.at(0));
	check(status);
}

void Context::setScene(const Scene& scene)
{
	int status;
	status = rprContextSetScene(instance(), scene.instance());
	check(status);
}

void Context::setAOV(const FrameBuffer& frameBuffer)
{
	int status;
	status = rprContextSetAOV(instance(), RPR_AOV_COLOR, frameBuffer.instance());
	check(status);
}

void Context::unsetAOV()
{
	int status;
	status = rprContextSetAOV(instance(), RPR_AOV_COLOR, nullptr);
	check(status);
}

void Context::setParameter1u(ContextInputType parameter, unsigned int value)
{
	int status;
	status = rprContextSetParameterByKey1u(instance(), static_cast<int>(parameter), value);
	check(status);
}

void Context::setParameter1f(ContextInputType parameter, float value)
{
	int status;
	status = rprContextSetParameterByKey1f(instance(), static_cast<int>(parameter), value);
	check(status);
}

void Context::render()
{
	int status;
	status = rprContextRender(instance());
	check(status);
}

void Context::resolve(FrameBuffer* src, FrameBuffer* dst, bool noDisplayGamma)
{
	int status;
	status = rprContextResolveFrameBuffer(instance(), src->instance(), dst->instance(), noDisplayGamma);
	check(status);
}

std::string Context::getCpuName() const
{
    int status;

    size_t dataSize;
    status = rprContextGetInfo(instance(), RPR_CONTEXT_CPU_NAME, 0, nullptr, &dataSize);
    check(status);

    if (dataSize == 0)
        return "";

    std::vector<char> buffer(dataSize);
    status = rprContextGetInfo(instance(), RPR_CONTEXT_CPU_NAME,
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
    status = rprContextGetInfo(instance(), rprFlag, 0, nullptr, &dataSize);
    check(status);

    if (dataSize == 0)
        return "";

    std::vector<char> buffer(dataSize);
    status = rprContextGetInfo(instance(), rprFlag,
            buffer.size(), buffer.data(), &dataSize);
    check(status);

    std::string deviceName;
    deviceName.reserve(dataSize-1);
    std::copy(buffer.begin(), buffer.end() - 1, std::back_inserter(deviceName));

    return deviceName;
}

std::filesystem::path Context::processHipKernelsPath(const std::filesystem::path& hipKernelsPath) const
{
    const std::vector<std::filesystem::path> pathHints {
        hipKernelsPath,
        "hipbin",
        "/usr/share/prorender/hipkernels"
    };
    const std::filesystem::path fileToCheck = "AllPreCompilations.json";

    for (const auto& hipDirectory : pathHints) {
        if (std::filesystem::exists(hipDirectory / fileToCheck)) {
            return hipDirectory;
        }
    }

    std::stringstream errorMessage;
    errorMessage << "Can't find hip file \"" << fileToCheck << "\" in directories: [ " << pathHints << " ]";

    throw Error(RPR_ERROR_IO_ERROR, errorMessage.str());
}

int Context::createFlags() const
{
    int flags;
    int status;

    size_t dataSize;
    status = rprContextGetInfo(instance(), RPR_CONTEXT_CREATION_FLAGS, sizeof(flags), &flags, &dataSize);
    check(status);

    if (dataSize != sizeof(flags))
        throw Error(RPR_ERROR_NULLPTR, "Can't get context creation flags");

    return flags;
}

}
