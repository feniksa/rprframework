#include "riff/Context.h"

#include <boost/program_options.hpp>
#include <iostream>
#include <boost/log/trivial.hpp>

namespace po = boost::program_options;
using namespace riff;

enum ErrorCode {
    Ok = 0,
    NotEnoughParams = -1,
    BadCompute = -2,
    BadDeviceIndex = -3,
    OtherError = -4,
    UnkownError = -5,
};

namespace
{

std::string allComputeEngines()
{
   std::stringstream s;

   s << to_string(BackendType::Openc) << ", ";
   s << to_string(BackendType::DirectX12) << ", ";
   s << to_string(BackendType::Metal) << "";

   return s.str();
}

}

int main(int argc, const char **argv) try
{
    // Declare the supported options.
    po::options_description desc("Generic options");
    desc.add_options()
            ("help", "show help message")
            ("compute", po::value<std::string>()->default_value("opencl"),("compute: " + allComputeEngines()).c_str())
            ("device", po::value<unsigned int>(), "device index to probe")
            ("api", po::value<uint64_t>()->default_value(RIF_API_VERSION), "force to use API version");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.contains("help")) {
        std::cout << desc << "\n";
        return ErrorCode::Ok;
    }

    BackendType backendType = BackendType::Openc;
    unsigned int deviceIndex = 0;

    // parse requirements for engine
    if (!vm.contains("compute")) {
        std::cerr << "Not enough params";
        return NotEnoughParams;
    }

    std::string computeName = vm["compute"].as<std::string>();

    // to lower case
    std::transform(computeName.begin(), computeName.end(), computeName.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    // parse string from cmd for render type: tahoe or northstar or hybrid
    bool result;
    std::tie(backendType, result) = from_string(computeName);
    if (!result) { // bad string
        std::cerr << "Unknown renderer engine. Supported renders are:" << allComputeEngines() << "\n";
        return ErrorCode::BadCompute;
    }

    if (vm.contains("device")) {
        deviceIndex = vm["device"].as<unsigned int>();
        if (deviceIndex >= 16) {
            std::cerr << "Bad device number " << deviceIndex << ". Device number should be in range [0-16]" << "\n";
            return ErrorCode::BadDeviceIndex;
        }
    }

    uint64_t version;
    if (vm.contains("api")) {
        version = vm["api"] .as<uint64_t>();
    } else {
        version = RIF_API_VERSION;
    }

    BOOST_LOG_TRIVIAL(debug) << "Create riff context " << to_string(backendType);
    riff::Context context(backendType, deviceIndex, "", version);
    BOOST_LOG_TRIVIAL(debug) << "OK";

    return 0;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << "\n";
    return ErrorCode::OtherError;
}
catch (...)
{
    std::cerr << "Unkown error\n";
    return ErrorCode::UnkownError;
}

