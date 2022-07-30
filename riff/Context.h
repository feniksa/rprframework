#pragma once

#include "ContextObject.h"
#include "RadeonImageFilterEnums.h"

namespace riff
{

class Context : public ContextObject<rif_context>
{
public:
	Context(BackendType backend, int device);

private:
};

} // namespace
