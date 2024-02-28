#pragma once

#include "rprf/MaterialNode.h"

namespace rprf
{

class UberMaterial : public MaterialNode
{
public:
	RPRF_API
	explicit UberMaterial(MaterialSystem& matsys);

	RPRF_API
	void SetDiffuseColor(float r, float g, float b, float a);

	RPRF_API
	void SetDiffuseColor(const MaterialNode& node);

	RPRF_API
	void SetDiffuseWeight(float r, float g, float b, float a);

	RPRF_API
	void SetDiffuseRoughness(float r, float g, float b, float a);

	RPRF_API
	void SetDiffuseNormal(const MaterialNode& node);


	// ----------- reflection ------------------------------------
	RPRF_API
	void SetReflectionColor(float r, float g, float b, float a);

	RPRF_API
	void SetReflectionColor(const MaterialNode& node);

	RPRF_API
	void SetReflectionWeight(float r, float g, float b, float a);

	RPRF_API
	void SetReflectionRoughness(float r, float g, float b, float a);

	// ----------- reflection ------------------------------------
	RPRF_API
	void SetEmissionColor(float r, float g, float b, float a);

	RPRF_API
	void SetEmissionWeight(float r, float g, float b, float a);

	RPRF_API
	void SetEmissionMode(UberMaterialEmissionMode mode);

private:
};

}