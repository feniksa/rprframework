#pragma once

#include "rprf/rprf.h"

namespace rprf
{

class Object
{
public:
	RPRF_API
	explicit Object(void* instance = nullptr) noexcept;

	RPRF_API
	Object(Object&& object) noexcept;

	RPRF_API
	virtual ~Object();

	RPRF_API
	void destroy() noexcept;

	RPRF_API
	Object& operator=(Object&& other) noexcept;

	Object(const Object&)            = delete;
	Object& operator=(const Object&) = delete;
protected:
	[[nodiscard]]
	RPRF_API
	void* instance() const noexcept { return m_instance; }
private:
	void* m_instance;
};

}
