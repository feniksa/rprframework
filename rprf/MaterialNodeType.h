#pragma once

#include <RadeonProRender.h>

namespace rprf
{
	enum class MaterialNodeType
	{
		Diffuse                         = RPR_MATERIAL_NODE_DIFFUSE,
		Microfacet                      = RPR_MATERIAL_NODE_MICROFACET,
		Reflection                      = RPR_MATERIAL_NODE_REFLECTION,
		Refraction                      = RPR_MATERIAL_NODE_REFRACTION,
		MicrofacetRefraction            = RPR_MATERIAL_NODE_MICROFACET_REFRACTION,
		Transparent                     = RPR_MATERIAL_NODE_TRANSPARENT,
		Emissive                        = RPR_MATERIAL_NODE_EMISSIVE,
		Ward                            = RPR_MATERIAL_NODE_WARD,
		Add                             = RPR_MATERIAL_NODE_ADD,
		Blend                           = RPR_MATERIAL_NODE_BLEND,
		Arithmetic                      = RPR_MATERIAL_NODE_ARITHMETIC,
		Fresnel                         = RPR_MATERIAL_NODE_FRESNEL,
		NormalMap                       = RPR_MATERIAL_NODE_NORMAL_MAP,
		ImageTexture                    = RPR_MATERIAL_NODE_IMAGE_TEXTURE,
		Noise2dTexture                  = RPR_MATERIAL_NODE_NOISE2D_TEXTURE,
		DotTexture                      = RPR_MATERIAL_NODE_DOT_TEXTURE,
		GradientTexture                 = RPR_MATERIAL_NODE_GRADIENT_TEXTURE,
		CheckerTexture                  = RPR_MATERIAL_NODE_CHECKER_TEXTURE,
		ConstantTexture                 = RPR_MATERIAL_NODE_CONSTANT_TEXTURE,
		InputLookup                     = RPR_MATERIAL_NODE_INPUT_LOOKUP,
		BlendValue                      = RPR_MATERIAL_NODE_BLEND_VALUE,
		Passthrough                     = RPR_MATERIAL_NODE_PASSTHROUGH,
		Orennayar                       = RPR_MATERIAL_NODE_ORENNAYAR,
		FresnelSchlick                  = RPR_MATERIAL_NODE_FRESNEL_SCHLICK,
		DiffuseRefraction               = RPR_MATERIAL_NODE_DIFFUSE_REFRACTION,
		BumpMap                         = RPR_MATERIAL_NODE_BUMP_MAP,
		Volume                          = RPR_MATERIAL_NODE_VOLUME,
		MicrofacetAnisotropicReflection = RPR_MATERIAL_NODE_MICROFACET_ANISOTROPIC_REFLECTION,
		MicrofacetAnisotropicRefraction = RPR_MATERIAL_NODE_MICROFACET_ANISOTROPIC_REFRACTION,
		Twosided                        = RPR_MATERIAL_NODE_TWOSIDED,
		UvProcedurar                    = RPR_MATERIAL_NODE_UV_PROCEDURAL,
		MicrofacetBeckmann              = RPR_MATERIAL_NODE_MICROFACET_BECKMANN,
		Phong                           = RPR_MATERIAL_NODE_PHONG,
		BufferSampler                   = RPR_MATERIAL_NODE_BUFFER_SAMPLER,
		UvTraplanar                     = RPR_MATERIAL_NODE_UV_TRIPLANAR,
		AoMap                           = RPR_MATERIAL_NODE_AO_MAP,
		UserTexture0                    = RPR_MATERIAL_NODE_USER_TEXTURE_0,
		UserTexture1                    = RPR_MATERIAL_NODE_USER_TEXTURE_1,
		UserTexture2                    = RPR_MATERIAL_NODE_USER_TEXTURE_2,
		UserTexture3                    = RPR_MATERIAL_NODE_USER_TEXTURE_3
	};
}
