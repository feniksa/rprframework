#pragma once

#include <RadeonProRender.h>
#include <stdexcept>

namespace rprf
{

class Error : public std::runtime_error
{
public:
	Error(rpr_int status);

	static const char* as_string(rpr_int status) noexcept;
private:
	rpr_int m_status;
};

void check(rpr_int status);

} // namespace
