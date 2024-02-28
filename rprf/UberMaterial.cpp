#include "rprf/UberMaterial.h"

namespace rprf
{
UberMaterial::UberMaterial(MaterialSystem& matsys)
: MaterialNode(matsys, MaterialNodeType::Uberv2)
{
}

void UberMaterial::SetDiffuseColor(float r, float g, float b, float a)
{
	setParameter4f(MaterialInputType::UberDiffuseColor, r, g, b, a);
}

void UberMaterial::SetDiffuseColor(const MaterialNode& node)
{
	setParameterNode(MaterialInputType::UberDiffuseColor, node);
}

void UberMaterial::SetDiffuseWeight(float r, float g, float b, float a)
{
	setParameter4f(MaterialInputType::UberDiffuseWeight, r, g, b, a);
}

void UberMaterial::SetDiffuseRoughness(float r, float g, float b, float a)
{
	setParameter4f(MaterialInputType::UberDiffuseRoughness, r, g, b, a);
}

void UberMaterial::SetDiffuseNormal(const rprf::MaterialNode &node)
{
	setParameterNode(MaterialInputType::UberDiffuseNormal, node);
}

void UberMaterial::SetReflectionColor(float r, float g, float b, float a)
{
	setParameter4f(MaterialInputType::UberReflectionColor, r, g, b, a);
}

void UberMaterial::SetReflectionColor(const MaterialNode& node)
{
	setParameterNode(MaterialInputType::UberReflectionColor, node);
}

void UberMaterial::SetReflectionWeight(float r, float g, float b, float a)
{
	setParameter4f(MaterialInputType::UberReflectionWeight, r, g, b, a);
}

void UberMaterial::SetReflectionRoughness(float r, float g, float b, float a)
{
	setParameter4f(MaterialInputType::UberReflectionRoughness, r, g, b, a);
}

void UberMaterial::SetEmissionColor(float r, float g, float b, float a)
{
	setParameter4f(MaterialInputType::UberEmissionColor, r, g, b, a);
}

void UberMaterial::SetEmissionWeight(float r, float g, float b, float a)
{
	setParameter4f(MaterialInputType::UberEmissionWeight, r, g, b, a);
}

void UberMaterial::SetEmissionMode(UberMaterialEmissionMode mode)
{
	setParameter1u(MaterialInputType::UberEmissionMode, static_cast<unsigned int>(mode));
}

} // namespace rprf

