#pragma once

#include "rprf/RadeonProRenderEnums.h"
#include "rprf/ContextObject.h"
#include "rprf/MaterialNodeInput.h"

namespace rprf
{

class MaterialSystem;
class Image;
class FrameBuffer;

class MaterialInputPin
{
public:
    using InputPins = std::vector<MaterialNodeInput>;

	RPRF_API
    MaterialInputPin() = default;

	RPRF_API
    MaterialInputPin(MaterialInputPin&&) = default;

	RPRF_API
    MaterialInputPin& operator=(const MaterialInputPin&) = default;

	RPRF_API
    MaterialInputPin& operator=(MaterialInputPin&&) = default;

	RPRF_API
    void reserve(size_t size)
    {
        m_inputPins.reserve(size);
    }

    template <class ... Args>
    void emplace_back(Args&& ... args) {
        m_inputPins.emplace_back(std::forward<Args>(args)...);
    }

private:
    InputPins m_inputPins;
};

class MaterialNode : public ContextObject
{
public:
    using InputPins = std::vector<MaterialNodeInput>;

	RPRF_API
	MaterialNode(MaterialSystem& matsys, MaterialNodeType type);

	RPRF_API
	void setParameter1u(MaterialInputType parameter, unsigned int x);

	RPRF_API
	void setParameter4f(MaterialInputType parameter, float x, float y, float z, float w);

	RPRF_API
	void setParameterNode(MaterialInputType parameter, const MaterialNode& node);

	RPRF_API
	void setParameterImage(MaterialInputType paramter, const Image& image);

	RPRF_API
	void setParameterFrameBuffer(MaterialInputType paramter, const FrameBuffer& frameBuffer);

	[[nodiscard]]
	RPRF_API
    InputPins readMaterialParameters() const;

	[[nodiscard]]
	RPRF_API
    std::string name() const;
};

RPRF_API
bool hasParameter(const MaterialNode::InputPins& inputPins, MaterialInputType parameter);

RPRF_API
std::tuple<float, float, float, float> getFloat4f(const MaterialNode::InputPins& inputPins,  MaterialInputType parameter);

RPRF_API
unsigned int getUInt(const MaterialNode::InputPins& inputPins,  MaterialInputType parameter);

RPRF_API
MaterialNode* getNode(const MaterialNode::InputPins& inputPins, MaterialInputType parameter);

} // namespace rprf
