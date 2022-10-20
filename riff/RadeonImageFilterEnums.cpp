#include "RadeonImageFilterEnums.h"
#include <stdexcept>
#include <algorithm>

namespace riff
{

std::string to_string(BackendType type)
{
    switch (type) {
        case BackendType::Metal:
            return "Metal";
        case BackendType::DirectX12:
            return "DirectX12";
        case BackendType::Openc:
            return "Opencl";
        default:
            throw std::runtime_error("riff::to_string(BackendType): unknown type");

    }
}

std::pair<BackendType, bool> from_string(std::string type)
{
    std::transform(type.begin(), type.end(), type.begin(),
               [](unsigned char c){ return std::tolower(c); });

    if (type == "opencl")
        return {BackendType::Openc, true};
     if (type == "directx12")
        return {BackendType::DirectX12, true};
     if (type == "metal")
        return {BackendType::Metal, true};

    return {BackendType::Openc, false};
}

} // namespace
