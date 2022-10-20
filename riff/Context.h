#pragma once

#include "ContextObject.h"
#include "RadeonImageFilterEnums.h"

namespace riff
{

class Context : public ContextObject<rif_context>
{
public:
	Context(BackendType backend, int device, std::string_view cacheDir = "", uint64_t version = RIF_API_VERSION);

private:
};

} // namespace
