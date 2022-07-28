#pragma once

#include "ContextObject.h"
#include "RprSupport.h"

namespace rpr
{
class MaterialSystem;

class Contextx : public ContextObject<rprx_context>
{
public:
	Contextx(const MaterialSystem& materialSystem);
	~Contextx();
};

}