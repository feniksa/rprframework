#include "rprf/MaterialNode.h"
#include "rprf/MaterialSystem.h"
#include "rprf/Image.h"
#include "rprf/FrameBuffer.h"
#include "rprf/Error.h"
#include <algorithm>
#include <cassert>

namespace rprf
{

MaterialNode::MaterialNode(MaterialSystem& matsys, MaterialNodeType type)
{
	rpr_int status;
	rpr_material_node node;
	status = rprMaterialSystemCreateNode(matsys.instance(), static_cast<int>(type), &node);
	check(status);

	setInstance(node);
    setCustomPointer(this);
}

void MaterialNode::setParameter1u(MaterialInputType parameter, unsigned int x)
{
	rpr_int status;
	status = rprMaterialNodeSetInputUByKey(instance(), static_cast<int>(parameter), x);
	check(status);
}

void MaterialNode::setParameter4f(MaterialInputType parameter, float x, float y, float z, float w)
{
	rpr_int status;
	status = rprMaterialNodeSetInputFByKey(instance(), static_cast<int>(parameter), x, y, z, w);
	check(status);
}

void MaterialNode::setParameterNode(MaterialInputType parameter, const MaterialNode& node)
{
	rpr_int status;
	status = rprMaterialNodeSetInputNByKey(instance(), static_cast<int>(parameter), node.instance());
	check(status);
}

void MaterialNode::setParameterImage(MaterialInputType parameter, const Image& image)
{
	rpr_int status;
	status = rprMaterialNodeSetInputImageDataByKey(instance(), static_cast<int>(parameter), image.instance());
	check(status);
}

void MaterialNode::setParameterFrameBuffer(MaterialInputType parameter, const FrameBuffer& frameBuffer)
{
	rpr_int status;
	status = rprMaterialNodeSetInputBufferDataByKey(instance(), static_cast<int>(parameter), frameBuffer.instance());
	check(status);
}

std::string MaterialNode::name() const
{
    int status;

    size_t stringSize = 0;
    status = rprMaterialNodeGetInfo(instance(), RPR_MATERIAL_NODE_NAME, 0, nullptr, &stringSize);
    check(status);

    std::string name;
    name.resize(stringSize);

    size_t readBytes = 0;
    status = rprMaterialNodeGetInfo(instance(), RPR_MATERIAL_NODE_NAME, name.size(), name.data(), &readBytes);
    check(status);

    assert(readBytes == name.size());
    if (readBytes > 0) {
        name.resize(name.size() - 1);
    }

    return name;
}

MaterialNode::InputPins MaterialNode::readMaterialParameters() const
{
    InputPins pins;

    rpr_int status;

    size_t numParams = 0;
    status = rprMaterialNodeGetInfo(instance(), RPR_MATERIAL_NODE_INPUT_COUNT, sizeof(numParams), &numParams, nullptr);
    check(status);

    // get the material type.
    rpr_material_node_type nodeType = 0;
    status = rprMaterialNodeGetInfo(instance(), RPR_MATERIAL_NODE_TYPE, sizeof(nodeType),&nodeType,nullptr);
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
    return iter != inputPins.end();
}

std::tuple<float, float, float, float> getFloat4f(const MaterialNode::InputPins& inputPins,  MaterialInputType parameter)
{
    const auto iter = std::find_if(inputPins.begin(), inputPins.end(), [parameter](const MaterialNodeInput& input){ return input.parameter() == parameter; });
    if (iter == inputPins.end())  {
        return {0.0f, 0.0f, 0.0f, 0.0f };
    }

    return iter->getFloat4();
}

MaterialNode* getNode(const MaterialNode::InputPins& inputPins, MaterialInputType parameter)
{
    const auto iter = std::find_if(inputPins.begin(), inputPins.end(), [parameter](const MaterialNodeInput& input){ return input.parameter() == parameter; });
    if (iter == inputPins.end())  {
        return nullptr;
    }

    MaterialNode* node = iter->getMaterialNode();
    return node;
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
