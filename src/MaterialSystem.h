#pragma once

#include "Context.h"

namespace rprf
{
class MaterialSystem : public ContextObject<rpr_material_system>
{
public:
	MaterialSystem(Context& context);
};

} // rpr
