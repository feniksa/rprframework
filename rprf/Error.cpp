#include "Error.h"
#include <unordered_map>
#include <sstream>

namespace rprf
{

Error::Error(rpr_int status)
: std::runtime_error(as_string(status)),
  m_status(status)
{
}
Error::Error(rpr_int status, std::string_view message)
: std::runtime_error(getErrorMessage(message, status)),
  m_status(status)
{
}

std::string Error::getErrorMessage(std::string_view message, rpr_int status)
{
	std::stringstream s;
	switch (status)
	{
	case RPR_ERROR_IO_ERROR:
		s << as_string(status) << ": Can't process file: " << message << "\n";
		break;
	default:
		s << as_string(status) << ": " << message << "\n";
		break;
	}
	return s.str();
}

const char* Error::as_string(rpr_int status) noexcept
{
	static const std::unordered_map<int, const char*> ErrorMap
	{
		{ RPR_ERROR_COMPUTE_API_NOT_SUPPORTED,         "Compute API not supported"         },
		{ RPR_ERROR_OUT_OF_SYSTEM_MEMORY,              "Out of system memory"              },
		{ RPR_ERROR_OUT_OF_VIDEO_MEMORY,               "Out of video memory"               },
		{ RPR_ERROR_SHADER_COMPILATION,                "Shader kernel error"               },
		{ RPR_ERROR_INVALID_LIGHTPATH_EXPR,            "Invalid lightpaht expr"            },
		{ RPR_ERROR_INVALID_IMAGE,                     "Invalid image"                     },
		{ RPR_ERROR_INVALID_AA_METHOD,                 "Invalid AA method"                 },
		{ RPR_ERROR_UNSUPPORTED_IMAGE_FORMAT,          "Unsupported image format"          },
		{ RPR_ERROR_INVALID_GL_TEXTURE,                "Invalid GL texture"                },
		{ RPR_ERROR_INVALID_CL_IMAGE,                  "Invalid CL image"                  },
		{ RPR_ERROR_INVALID_OBJECT,                    "Invalid object"                    },
		{ RPR_ERROR_INVALID_PARAMETER,                 "Invalid parameter"                 },
		{ RPR_ERROR_INVALID_TAG,                       "Invalid tag"                       },
		{ RPR_ERROR_INVALID_LIGHT,                     "Invalid light"                     },
		{ RPR_ERROR_INVALID_CONTEXT,                   "Invalid context"                   },
		{ RPR_ERROR_UNIMPLEMENTED,                     "Not implemented"                   },
		{ RPR_ERROR_INVALID_API_VERSION,               "Invalid API version"               },
		{ RPR_ERROR_INTERNAL_ERROR,                    "Internal error"                    },
		{ RPR_ERROR_IO_ERROR,                          "I/O error"                         },
		{ RPR_ERROR_UNSUPPORTED_SHADER_PARAMETER_TYPE, "Unsupported shader parameter type" },
		{ RPR_ERROR_MATERIAL_STACK_OVERFLOW,           "Material stack overflow"           },
		{ RPR_ERROR_INVALID_PARAMETER_TYPE,            "Invalid parameter type"            },
		{ RPR_ERROR_UNSUPPORTED,                       "Unsupported"                       },
		{ RPR_ERROR_OPENCL_OUT_OF_HOST_MEMORY,         "OpenCL out of host memory"         },
		{ RPR_ERROR_OPENGL ,                           "OpenGL error"                      },
		{ RPR_ERROR_OPENCL,                            "OpenCL error"                      },
		{ RPR_ERROR_NULLPTR ,                          "Nullptr error"                     },
		{ RPR_ERROR_NODETYPE,                          "Invalid node type"                 },
		{ RPR_ERROR_ABORTED,                           "Error: aborted"                    },
	};

	const char* defaultErrorMessage = "RadeonProRender: Unknown Error";

	auto iterator = ErrorMap.find(status);
	return (iterator != ErrorMap.end()) ? iterator->second: defaultErrorMessage;
}


} // namespace
