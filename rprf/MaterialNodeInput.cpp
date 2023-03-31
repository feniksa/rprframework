#include "MaterialNodeInput.h"
#include "MaterialNode.h"
#include <cassert>

namespace rprf
{
MaterialNodeInput::MaterialNodeInput(const MaterialNode& node, size_t index) :
m_parameter(readParameter(node, index)),
m_dataType(readDataType(node, index)),
 m_data(readData(node, index))
{
}

float MaterialNodeInput::getFloat4() const
{
    assert(m_dataType == MaterialNodeInputDataType::Float4);

    const float* value = reinterpret_cast<const float*>(m_data.data());
    return *value;
}

const rpr_material_node* MaterialNodeInput::getMaterialNode() const
{
    assert(m_dataType == MaterialNodeInputDataType::Node);

    const rpr_material_node *mat = reinterpret_cast<const rpr_material_node *>(m_data.data());
    return mat;
}

MaterialInputType MaterialNodeInput::readParameter(const MaterialNode& node, unsigned int index)
{
    unsigned int pinName;

    static_assert(sizeof(rpr_material_node_input) == sizeof(pinName));

    rpr_status status;
    status = rprMaterialNodeGetInputInfo(node.instance(), index, RPR_MATERIAL_NODE_INPUT_NAME, sizeof(pinName), &pinName, nullptr);
    check(status);

    return static_cast<MaterialInputType>(pinName);
}

MaterialNodeInputDataType MaterialNodeInput::readDataType(const MaterialNode& node, unsigned  int index)
{
    MaterialNodeInputDataType inputType;
    rpr_status status;

    status = rprMaterialNodeGetInputInfo(node.instance(), index, RPR_MATERIAL_NODE_INPUT_TYPE, sizeof(inputType), &inputType, nullptr);
    check(status);

    return inputType;
}

std::vector<std::byte> MaterialNodeInput::readData(const MaterialNode& node, unsigned int index)
{
    rpr_status status;

    size_t inputValueSizeB = 0;
    status = rprMaterialNodeGetInputInfo(node.instance(), index, RPR_MATERIAL_NODE_INPUT_VALUE, 0, nullptr, &inputValueSizeB);
    check(status);

    std::vector<std::byte> data(inputValueSizeB);

    size_t actualReadSize = 0;
    status = rprMaterialNodeGetInputInfo(node.instance(), index, RPR_MATERIAL_NODE_INPUT_VALUE, inputValueSizeB, data.data(), &actualReadSize);
    check(status);

#ifndef NDEBUG
    if(actualReadSize != inputValueSizeB)
        throw Error(RPR_ERROR_ABORTED, "Can't read material input data from RPR");
#endif

    return data;
}

std::ostream& operator<<(std::ostream& stream, const MaterialNodeInput& materialNodeInput)
{
    stream << "" << static_cast<int>(materialNodeInput.parameter()) << "\n";

    switch(materialNodeInput.dataType()) {
        case MaterialNodeInputDataType::Float4:
            stream << "float4(" << materialNodeInput.getFloat4() << ")\n";
            break;
        case MaterialNodeInputDataType::Node:
            stream << "node(" << materialNodeInput.getMaterialNode() << ")\n";
            break;
        default:
            stream << "unknown type\n";
            break;
    }

    return stream;
}

}