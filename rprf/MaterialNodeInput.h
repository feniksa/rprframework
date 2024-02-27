#pragma once

#include "rprf/rprf.h"
#include "rprf/RadeonProRenderEnums.h"
#include <vector>
#include <ostream>
#include <tuple>

namespace rprf
{

class MaterialNode;

class MaterialNodeInput
{
public:
	RPRF_API
    MaterialNodeInput() = default;

	RPRF_API
    MaterialNodeInput(const MaterialNode& node, size_t index);

	[[nodiscard]]
	RPRF_API
    MaterialInputType parameter() const noexcept { return m_parameter; }

	[[nodiscard]]
	RPRF_API
    MaterialNodeInputType dataType() const noexcept { return m_dataType; }

	[[nodiscard]]
	RPRF_API
    std::tuple<float, float, float, float> getFloat4() const;

	[[nodiscard]]
	RPRF_API
	unsigned int getUInt() const;

	[[nodiscard]]
	RPRF_API
    MaterialNode* getMaterialNode() const;

private:
	static
    MaterialInputType readParameter(const MaterialNode& node, unsigned int index);

	static
    MaterialNodeInputType readDataType(const MaterialNode& node, unsigned  int index);

	static
    std::vector<std::byte> readData(const MaterialNode& node, unsigned int index);

    MaterialInputType m_parameter;
    MaterialNodeInputType m_dataType;
    std::vector<std::byte> m_data;
};

RPRF_API
std::ostream& operator<<(std::ostream& stream, const MaterialNodeInput& materialNodeInput);

} // namespace