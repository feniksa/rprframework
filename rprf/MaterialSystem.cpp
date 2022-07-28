#include "MaterialSystem.h"
#include "Error.h"

namespace rprf
{

MaterialSystem::MaterialSystem(Context& context)
{
	int status;
	rpr_material_system matsys;
	status = rprContextCreateMaterialSystem(context.instance(), 0, &matsys);
	check(status);

	setInstance(std::move(matsys));
}

} // rpr
