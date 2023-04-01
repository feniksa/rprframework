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

std::tuple<float, float, float, float> MaterialNodeInput::getFloat4() const
{
    assert(m_dataType == MaterialNodeInputDataType::Float4);

    const float* float1 = reinterpret_cast<const float*>(m_data.data() + sizeof(float) * 0);
    const float* float2 = reinterpret_cast<const float*>(m_data.data() + sizeof(float) * 1);
    const float* float3 = reinterpret_cast<const float*>(m_data.data() + sizeof(float) * 2);
    const float* float4 = reinterpret_cast<const float*>(m_data.data() + sizeof(float) * 3);

    return {*float1, *float2, *float3, *float4};
}

unsigned int MaterialNodeInput::getUInt() const
{
    assert(m_dataType == MaterialNodeInputDataType::Uint);

    const unsigned int* number = reinterpret_cast<const unsigned int*>(m_data.data());
    return *number;
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
        case MaterialNodeInputDataType::Float4: {
            float r, g, b, a;
            std::tie(r, g, b, a) = materialNodeInput.getFloat4();
            stream << "float4(" << r << ", " << g << ", " << b << ", " << a << ")\n";
            break;
        }
        case MaterialNodeInputDataType::Uint: {
            const unsigned int uintNumber = materialNodeInput.getUInt();
            stream << "uint(" << uintNumber << ")\n";
            break;
        }
        case MaterialNodeInputDataType::Node: {
            stream << "node(" << materialNodeInput.getMaterialNode() << ")\n";
            break;
        }
        default:
            stream << "unknown type\n";
            break;
    }

    return stream;
}

}