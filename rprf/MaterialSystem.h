#pragma once

#include "rprf/Context.h"
#include "rprf/CastContainer.hpp"

namespace rprf
{
class MaterialNode;

class MaterialSystem : public ContextObject
{
public:
	RPRF_API
	explicit MaterialSystem(Context& context);

	[[nodiscard]]
	RPRF_API
    CastContainer<MaterialNode> getNodeList() const;
private:
    Context& m_context;
};

} // rpr
