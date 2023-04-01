#pragma once

#include "RadeonProRenderEnums.h"
#include "ContextObject.h"
#include "MaterialNodeInput.h"

namespace rprf
{

class MaterialSystem;
class Image;
class FrameBuffer;

class MaterialInputPin
{
public:
    using InputPins = std::vector<MaterialNodeInput>;

    MaterialInputPin() = default;
    MaterialInputPin(MaterialInputPin&&) = default;

    MaterialInputPin& operator=(const MaterialInputPin&) = default;
    MaterialInputPin& operator=(MaterialInputPin&&) = default;

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

class MaterialNode : public ContextObject<rpr_material_node>
{
public:
    using InputPins = std::vector<MaterialNodeInput>;

	MaterialNode(MaterialSystem& matsys, MaterialNodeType type);

	void setParameter1u(MaterialInputType parameter, unsigned int x);
	void setParameter4f(MaterialInputType parameter, float x, float y, float z, float w);

	void setParameterNode(MaterialInputType parameter, const MaterialNode& node);
	void setParameterImage(MaterialInputType paramter, const Image& image);
	void setParameterFrameBuffer(MaterialInputType paramter, const FrameBuffer& frameBuffer);

    InputPins readMaterialParameters() const;
};

bool hasParameter(const MaterialNode::InputPins& inputPins, MaterialInputType parameter);
std::tuple<float, float, float, float> getFloat4f(const MaterialNode::InputPins& inputPins,  MaterialInputType parameter);
unsigned int getUInt(const MaterialNode::InputPins& inputPins,  MaterialInputType parameter);
const rpr_material_node* getNode(const MaterialNode::InputPins& inputPins, MaterialInputType parameter);

}
