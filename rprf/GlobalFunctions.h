#pragma once

#include "rprf/rprf.h"

namespace rprf
{
	namespace sdk
	{
		RPRF_API
		void rprObjectDelete(void*) noexcept;

		RPRF_API
		void rprObjectSetName(void*, const char*);

		RPRF_API
		void rprObjectSetCustomPointer(void*, void*);

		RPRF_API
		const void* rprObjectGetCustomPointer (void*);
	}
} // namespace rprf