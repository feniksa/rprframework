#pragma once

#include "Context.h"
#include "CastContainer.h"

namespace rprf
{
    class MaterialNode;

class MaterialSystem : public ContextObject<rpr_material_system>
{
public:
	MaterialSystem(Context& context);

    CastContainer<MaterialNode> getNodeList() const;

private:
    Context& m_context;
};

} // rpr
