#pragma once

#include "rprf/rprf.h"
#include <RadeonProRender.h>
#include <stdexcept>

namespace rprf
{

class Error : public std::runtime_error
{
public:
	RPRF_API
	explicit Error(rpr_int status);

	RPRF_API
	explicit Error(rpr_int status, std::string_view message);

	RPRF_API
	static const char* as_string(rpr_int status) noexcept;

	[[nodiscard]]
	RPRF_API
    int status() const noexcept { return m_status; }
private:
	static std::string getErrorMessage(std::string_view message, rpr_int status);
	rpr_int m_status;
};

inline void check(rpr_int status)
{
	if (status != RPR_SUCCESS)
		throw Error(status);
}

inline void check(rpr_int status, std::string_view message)
{
	if (status != RPR_SUCCESS)
		throw Error(status, message);
}

} // namespace
