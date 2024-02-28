#pragma once

#include "rprf/rprf.h"

namespace rprf
{

class ContextObject
{
public:
	RPRF_API
	explicit ContextObject(void* data = nullptr) noexcept;

	RPRF_API
	ContextObject(ContextObject&& object)  noexcept;

	RPRF_API
	virtual ~ContextObject() noexcept;

	RPRF_API
	ContextObject& operator=(ContextObject&& context) noexcept;

	[[nodiscard]]
	RPRF_API
	void* instance() const noexcept { return m_instance; }

	RPRF_API
    void setName(const char* name);

	[[nodiscard]]
	RPRF_API
    const void* getCustomPointer() const;

	RPRF_API
	void destroy() noexcept;

	// we don't allow to copy context object
	ContextObject(const ContextObject&)            = delete;
	ContextObject& operator=(const ContextObject&) = delete;

protected:
    void setCustomPointer(void* pointer);
    void setInstance(void* instance);

private:
	void* m_instance;
};

} // namespace
