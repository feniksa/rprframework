#include "Error.h"
#include <unordered_map>
#include <sstream>

namespace riff
{

Error::Error(rif_int status)
: std::runtime_error(as_string(status)),
  m_status(status)
{
}
Error::Error(rif_int status, std::string_view message)
	: std::runtime_error(getErrorMessage(message, status)),
	m_status(status)
{
}

std::string Error::getErrorMessage(std::string_view message, rif_int status)
{
	std::stringstream s;
	switch (status)
	{
	case RIF_ERROR_IO_ERROR:
		s << as_string(status) << ": Can't process file: " << message << "\n";
		break;
	default:
		s << as_string(status) << ": " << message << "\n";
		break;
	}
	return s.str();
}


const char* Error::as_string(rif_int status) noexcept
{
	static const std::unordered_map<int, const char*> ErrorMap
	{
		{ RIF_ERROR_OUT_OF_VIDEO_MEMORY,              "out of memory"                    },
		{ RIF_ERROR_INVALID_IMAGE,                    "invalid image"                    },
		{ RIF_ERROR_UNSUPPORTED_IMAGE_FORMAT,         "unsupported image format"         },
		{ RIF_ERROR_INVALID_GL_TEXTURE,               "invalid opengl texture"           },
		{ RIF_ERROR_INVALID_CL_IMAGE,                 "invalid opencl image"             },
		{ RIF_ERROR_INVALID_OBJECT,                   "invalid object"                   },
		{ RIF_ERROR_INVALID_PARAMETER,                "invalid parameter"                },
		{ RIF_ERROR_INVALID_CONTEXT,                  "invalid context"                  },
		{ RIF_ERROR_INVALID_QUEUE,                    "invalid queue"                    },
		{ RIF_ERROR_INVALID_FILTER,                   "invalid filter"                   },
		{ RIF_ERROR_INVALID_FILTER_ARGUMENT_NAME,     "invalid filter argument name"     },
		{ RIF_ERROR_UNIMPLEMENTED,                    "unimplemented"                    },
		{ RIF_ERROR_INVALID_API_VERSION,              "invalid api version"              },
		{ RIF_ERROR_INTERNAL_ERROR,                   "internal error"                   },
		{ RIF_ERROR_IO_ERROR,                         "i/o error"                        },
		{ RIF_ERROR_INVALID_PARAMETER_TYPE,           "invalid parameter type"           },
		{ RIF_ERROR_UNSUPPORTED,                      "unsupported"                      },
	};

	auto iterator = ErrorMap.find(status);
	return (iterator != ErrorMap.end()) ? iterator->second : "RadeonImageFilter: Unkown Error";
}

} // namespace
