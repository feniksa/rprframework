#pragma once

#include "MaterialNodeType.h"
#include "ContextObject.h"

namespace rprf
{

class MaterialSystem;
class Image;
class FrameBuffer;

class MaterialNode : public ContextObject<rpr_material_node>
{
public:
	MaterialNode(MaterialSystem& matsys, MaterialNodeType type);

	void setParameter1u(unsigned int parameter, unsigned int x);
	void setParameter4f(unsigned int parameter, float x, float y, float z, float w);

	void setParameterNode(unsigned int parameter, const MaterialNode& node);
	void setParameterImage(unsigned int paramter, const Image& image);
	void setParameterFrameBuffer(unsigned int paramter, const FrameBuffer& frameBuffer);
};

}
