#include "MaterialSystem.h"
#include "Error.h"
#include <cassert>

namespace rprf
{

MaterialSystem::MaterialSystem(Context& context)
: m_context(context)
{
	int status;
	rpr_material_system matsys;
	status = rprContextCreateMaterialSystem(context.instance(), 0, &matsys);
	check(status);

	setInstance(std::move(matsys));
}

CastContainer<MaterialNode> MaterialSystem::getNodeList() const
{
    int status;
    rpr_context_info info = RPR_CONTEXT_LIST_CREATED_MATERIALNODES;
    size_t returnSize;

    status = rprContextGetInfo(m_context.instance(), info, 0, nullptr, &returnSize);
    check(status);

    CastContainer<MaterialNode> materialNodes(returnSize / sizeof(rpr_material_node));
    status = rprContextGetInfo(m_context.instance(), info, materialNodes.size() * sizeof(rpr_material_node), materialNodes.data(), &returnSize);
    check(status);

    assert(returnSize == materialNodes.size() * sizeof(rpr_material_node));

    return materialNodes;

    /*for (rpr_material_node node : materialNodes) {
        const void* ptr;
        status = rprObjectGetCustomPointer(node, &ptr);
        check(status);
    }*/

    /*unsigned int nodesCount;
    status = rprMaterialSystemGetSize(instance(), &nodesCount);
    check(status);

    status = rprMaterialSystemGetInfo(instance(), info, 0, nullptr, &returnSize);
    check(status);

    std::vector<rpr_material_node*> nodes(returnSize);
    if (nodes.empty())
        return;

    status = rprMaterialSystemGetInfo(instance(), info, sizeof (info), nodes.data(), &returnSize);
    check(status);*/
}

} // rpr
