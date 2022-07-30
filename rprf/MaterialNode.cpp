#include "MaterialNode.h"
#include "MaterialSystem.h"
#include "Image.h"
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


void MaterialNode::setParameter1u(MaterialInputType parameter, unsigned int x)
{
	rpr_int status;
	status = rprMaterialNodeSetInputUByKey(*this, static_cast<int>(parameter), x);
	check(status);
}

void MaterialNode::setParameter4f(MaterialInputType parameter, float x, float y, float z, float w)
{
	rpr_int status;
	status = rprMaterialNodeSetInputFByKey(*this, static_cast<int>(parameter), x, y, z, w);
	check(status);
}

void MaterialNode::setParameterNode(MaterialInputType parameter, const MaterialNode& node)
{
	rpr_int status;
	status = rprMaterialNodeSetInputNByKey(*this, static_cast<int>(parameter), node.instance());
	check(status);
}

void MaterialNode::setParameterImage(MaterialInputType parameter, const Image& image)
{
	rpr_int status;
	status = rprMaterialNodeSetInputImageDataByKey(*this, static_cast<int>(parameter), image.instance());
	check(status);
}

void MaterialNode::setParameterFrameBuffer(MaterialInputType parameter, const FrameBuffer& frameBuffer)
{
	rpr_int status;
	status = rprMaterialNodeSetInputBufferDataByKey(*this, static_cast<int>(parameter), frameBuffer.instance());
	check(status);
}

}