#pragma once

#include "rprf/rprf.h"
#include "rprf/ContextObject.h"
#include "rprf/RadeonProRenderEnums.h"
#include <filesystem>

namespace rprf
{
class Plugin;
class Scene;
class FrameBuffer;

class Context : public ContextObject
{
public:
	RPRF_API
	explicit Context(const Plugin& plugin, const std::filesystem::path& cachePath = "",
					 const std::filesystem::path& hipKernelsPath = "",
					 int createFlags = RPR_CREATION_FLAGS_ENABLE_GPU0,
					 unsigned int apiVersion = RPR_API_VERSION);

	RPRF_API
	void setScene(const Scene& scene);

	RPRF_API
	void setAOV(const FrameBuffer& frameBuffer);

	RPRF_API
	void unsetAOV();

	RPRF_API
	void resolve(FrameBuffer* src, FrameBuffer* dst, bool noDisplayGamma = true);

	RPRF_API
	void setParameter1u(ContextInputType parameter, unsigned int value);

	RPRF_API
	void setParameter1f(ContextInputType parameter, float value);

	RPRF_API
	void render();

	[[nodiscard]]
	RPRF_API
    std::string getCpuName() const;

	[[nodiscard]]
	RPRF_API
    std::string getGpuName(int index) const;

	[[nodiscard]]
	RPRF_API
	int createFlags() const;
private:
	[[nodiscard]]
    std::filesystem::path processHipKernelsPath(const std::filesystem::path& path) const;
};

}
