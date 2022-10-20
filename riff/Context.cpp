#include "Context.h"
#include "Error.h"

namespace riff
{

Context::Context(BackendType backend, int device, std::string_view cache_path, uint64_t version)
{
	rif_int status;
	rif_context context;

	status  = rifCreateContext(version, static_cast<int>(backend), device, cache_path.data(), &context);
	check(status);

	setInstance(std::move(context));
}

} // namespace
