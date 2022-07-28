#include "Contextx.h"
#include "MaterialSystem.h"
#include "Error.h"

namespace rpr
{

Contextx::Contextx(const MaterialSystem& matsys)
{
	int status;
	rprx_context context;
	status = rprxCreateContext(matsys, RPRX_FLAGS_ENABLE_LOGGING, &context);
	check(status);

	setInstance(std::move(context));
}

Contextx::~Contextx() 
{
	if (!instance())
		return;

	int status;
	status = rprxDeleteContext(instance());
	check(status);

	setInstanceToNull();
}

} // rpr