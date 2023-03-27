#pragma once

#include "RadeonProRenderEnums.h"
#include "ContextObject.h"
#include "MaterialNodeInput.h"

namespace rprf
{

class MaterialSystem;
class Image;
class FrameBuffer;

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

    InputPins readInputPins() const;
};

}
