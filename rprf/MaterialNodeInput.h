#pragma once

#include "RadeonProRenderEnums.h"
#include <vector>
#include <ostream>
#include <tuple>

namespace rprf
{

class MaterialNode;

class MaterialNodeInput
{
public:
    explicit MaterialNodeInput() = default;
    explicit MaterialNodeInput(const MaterialNode& node, size_t index);

    MaterialInputType parameter() const noexcept { return m_parameter; }
    MaterialNodeInputType dataType() const noexcept { return m_dataType; }

    std::tuple<float, float, float, float> getFloat4() const;
    unsigned int getUInt() const;
    const rpr_material_node* getMaterialNode() const;

private:
    MaterialInputType readParameter(const MaterialNode& node, unsigned int index);
    MaterialNodeInputType readDataType(const MaterialNode& node, unsigned  int index);
    std::vector<std::byte> readData(const MaterialNode& node, unsigned int index);

    MaterialInputType m_parameter;
    MaterialNodeInputType m_dataType;
    std::vector<std::byte> m_data;
};

std::ostream& operator<<(std::ostream& stream, const MaterialNodeInput& materialNodeInput);

} // namespace