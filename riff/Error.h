#pragma once

#include <RadeonImageFilters.h>
#include <stdexcept>

namespace riff
{

class Error : public std::runtime_error
{
public:
	explicit Error(rif_int status);
	explicit Error(rif_int status, std::string_view message);

    rif_int status() const noexcept { return m_status; }

	static const char* as_string(rif_int status) noexcept;
private:
	static std::string getErrorMessage(std::string_view message, rif_int status);
	rif_int m_status;
};

inline void check(rif_int status)
{
	if (status != RIF_SUCCESS)
		throw Error(status);
}

inline void check(rif_int status, std::string_view message)
{
	if (status != RIF_SUCCESS)
		throw Error(status, message);
}


} // namespace
