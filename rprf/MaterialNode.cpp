#include "MaterialNode.h"
#include "MaterialSystem.h"
#include "Image.h"
#include "FrameBuffer.h"
#include "Error.h"
#include <algorithm>

namespace rprf
{

MaterialNode::MaterialNode(MaterialSystem& matsys, MaterialNodeType type)
{
	rpr_int status;
	rpr_material_node node;
	status = rprMaterialSystemCreateNode(matsys.instance(), static_cast<int>(type), &node);
	check(status);

	setInstance(std::move(node));
}

void MaterialNode::setParameter1u(MaterialInputType parameter, unsigned int x)
{
	rpr_int status;
	status = rprMaterialNodeSetInputUByKey(*this, static_cast<int>(parameter), x);
	check(status);
}

void MaterialNode::setParameter4f(MaterialInputType parameter, float x, float y, float z, float w)
{
	rpr_int status;
	status = rprMaterialNodeSetInputFByKey(*this, static_cast<int>(parameter), x, y, z, w);
	check(status);
}

void MaterialNode::setParameterNode(MaterialInputType parameter, const MaterialNode& node)
{
	rpr_int status;
	status = rprMaterialNodeSetInputNByKey(*this, static_cast<int>(parameter), node.instance());
	check(status);
}

void MaterialNode::setParameterImage(MaterialInputType parameter, const Image& image)
{
	rpr_int status;
	status = rprMaterialNodeSetInputImageDataByKey(*this, static_cast<int>(parameter), image.instance());
	check(status);
}

void MaterialNode::setParameterFrameBuffer(MaterialInputType parameter, const FrameBuffer& frameBuffer)
{
	rpr_int status;
	status = rprMaterialNodeSetInputBufferDataByKey(*this, static_cast<int>(parameter), frameBuffer.instance());
	check(status);
}

MaterialNode::InputPins MaterialNode::readMaterialParameters() const
{
    InputPins pins;

    rpr_int status;

    size_t numParams = 0;
    status = rprMaterialNodeGetInfo(*this, RPR_MATERIAL_NODE_INPUT_COUNT, sizeof(numParams), &numParams, nullptr);
    check(status);

    // get the material type.
    rpr_material_node_type nodeType = 0;
    status = rprMaterialNodeGetInfo(*this,RPR_MATERIAL_NODE_TYPE, sizeof(nodeType),&nodeType,nullptr);
    check(status);

    pins.reserve(numParams);

    // for each parameter
    for (size_t index = 0; index < numParams; ++index) {
        pins.emplace_back(*this, index);
    }

    return pins;
}


// helper functions
bool hasParameter(const MaterialNode::InputPins& inputPins, MaterialInputType parameter)
{
    const auto iter = std::find_if(inputPins.begin(), inputPins.end(), [parameter](const MaterialNodeInput& input){ return input.parameter() == parameter; });
    return (iter != inputPins.end()) ? true : false;
}

std::tuple<float, float, float, float> getFloat4f(const MaterialNode::InputPins& inputPins,  MaterialInputType parameter)
{
    const auto iter = std::find_if(inputPins.begin(), inputPins.end(), [parameter](const MaterialNodeInput& input){ return input.parameter() == parameter; });
    if (iter == inputPins.end())  {
        return {0.0f, 0.0f, 0.0f, 0.0f };
    }

    return iter->getFloat4();
}

const rpr_material_node* getNode(const MaterialNode::InputPins& inputPins, MaterialInputType parameter)
{
    const auto iter = std::find_if(inputPins.begin(), inputPins.end(), [parameter](const MaterialNodeInput& input){ return input.parameter() == parameter; });
    if (iter == inputPins.end())  {
        return nullptr;
    }

    return iter->getMaterialNode();
}

unsigned int getUInt(const MaterialNode::InputPins& inputPins,  MaterialInputType parameter)
{
    const auto iter = std::find_if(inputPins.begin(), inputPins.end(), [parameter](const MaterialNodeInput& input){ return input.parameter() == parameter; });
    if (iter == inputPins.end())  {
        return 0;
    }

    return iter->getUInt();
}

}
