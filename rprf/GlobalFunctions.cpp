#include "rprf/GlobalFunctions.h"
#include "rprf/Error.h"
#include <iostream>
#include <cassert>

namespace rprf
{
	void __rprObjectDelete(void* instance) noexcept
	{
		assert(instance);

		try {
			rpr_int status;
			status = rprObjectDelete(instance);
			check(status);
		}
		catch(const std::exception& e)
		{
			std::cerr << "ContextObject::destroy(): " << e.what();
		}
		catch (...)
		{
			std::cerr << "ContextObject::destroy(): unknown error";
		}
	}

	void __rprObjectSetName(void* instance, const char* name)
	{
		assert(instance);
		rpr_int status;
		status = rprObjectSetName(instance, name);
		check(status);
	}

	void __rprObjectSetCustomPointer(void* instance, void* pointer)
	{
		assert(instance);
		rpr_int status;
		status = rprObjectSetCustomPointer(instance, pointer);
		check(status);
	}

	const void* __rprObjectGetCustomPointer(void* instance)
	{
		assert(instance);

		const void* pointer;

		rpr_int status;
		status = rprObjectGetCustomPointer(instance, &pointer);
		check(status);

		return pointer;
	}
}