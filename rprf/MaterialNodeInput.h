#pragma once

#include "RadeonProRenderEnums.h"
#include <vector>
#include <ostream>

namespace rprf
{

class MaterialNode;

class MaterialNodeInput
{
public:
    explicit MaterialNodeInput() = default;
    explicit MaterialNodeInput(const MaterialNode& node, size_t index);

    unsigned int pinName() const { return m_pinName; }
    MaterialNodeInputType type() const { return m_type; }

    float getFloat4() const;
    const rpr_material_node* getMaterialNode() const;

private:
    unsigned int readPinName(const MaterialNode& node, unsigned int index);
    MaterialNodeInputType readInputType(const MaterialNode& node, unsigned  int index);
    std::vector<std::byte> readInputValue(const MaterialNode& node, unsigned int index);

    unsigned int m_pinName;
    MaterialNodeInputType m_type;
    std::vector<std::byte> m_data;
};

std::ostream& operator<<(std::ostream& stream, const MaterialNodeInput& materialNodeInput);

} // namespace