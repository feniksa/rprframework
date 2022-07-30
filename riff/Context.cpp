#include "Context.h"
#include "Error.h"

namespace riff
{

Context::Context(BackendType backend, int device)
{
	rif_int status;
	rif_context context;

	status  = rifCreateContext(RIF_API_VERSION, static_cast<int>(backend), device, nullptr, &context);
	check(status);

	setInstance(std::move(context));
}

} // namespace
