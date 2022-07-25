#include "MaterialNode.h"
#include "MaterialSystem.h"
//#include "Image.h"
#include "FrameBuffer.h"
#include "Error.h"

namespace rprf
{

MaterialNode::MaterialNode(MaterialSystem& matsys, MaterialNodeType type)
{
	rpr_int status;
	rpr_material_node node;
	status = rprMaterialSystemCreateNode(matsys.instance(), static_cast<int>(type), &node);
	check(status);

	setInstance(std::move(node));
}


void MaterialNode::setParameter1u(unsigned int parameter, unsigned int x)
{
	rpr_int status;
	status = rprMaterialNodeSetInputUByKey(*this, parameter, x);
	check(status);
}

void MaterialNode::setParameter4f(unsigned int parameter, float x, float y, float z, float w)
{
	rpr_int status;
	status = rprMaterialNodeSetInputFByKey(*this, parameter, x, y, z, w);
	check(status);
}

void MaterialNode::setParameterNode(unsigned int parameter, const MaterialNode& node)
{
	rpr_int status;
	status = rprMaterialNodeSetInputNByKey(*this, parameter, node.instance());
	check(status);
}

void MaterialNode::setParameterImage(unsigned int parameter, const Image& image)
{
	/*rpr_int status;
	status = rprMaterialNodeSetInputImageData(*this, parameter, image.instance());
	check(status);*/
}

void MaterialNode::setParameterFrameBuffer(unsigned int parameter, const FrameBuffer& frameBuffer)
{
	rpr_int status;
	status = rprMaterialNodeSetInputBufferDataByKey(*this, parameter, frameBuffer.instance());
	check(status);
}


}
