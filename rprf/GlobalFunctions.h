#pragma once

#include "rprf/rprf.h"

namespace rprf
{

RPRF_API
void __rprObjectDelete(void*) noexcept;

RPRF_API
void __rprObjectSetName(void*, const char*);

RPRF_API
void __rprObjectSetCustomPointer(void*, void*);

RPRF_API
const void* __rprObjectGetCustomPointer (void*);

} // namespace rprf