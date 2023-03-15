#include "riff/Context.h"

#include "riff/ContextUtils.h"

#include <boost/program_options.hpp>
#include <iostream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

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
    std::string computeName;
    unsigned int deviceIndex;

    // Declare the supported options.
    po::options_description desc("Generic options");
    desc.add_options()
            ("help", "show help message")
            ("compute", po::value<std::string>(&computeName)->default_value("opencl"),("compute: " + allComputeEngines()).c_str())
            ("device", po::value<unsigned int>(&deviceIndex)->default_value(0), "device index to probe")
            ("verbosity", po::value<std::string>(), "verbosity")
            ("api", po::value<uint64_t>()->default_value(RIF_API_VERSION), "force to use API version");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.contains("help")) {
        std::cout << desc << "\n";
        return ErrorCode::Ok;
    }

    if (vm.contains("verbosity")) {
        std::string verbosity = vm["verbosity"].as<std::string>();

        boost::log::trivial::severity_level severityLevel;
        boost::log::trivial::from_string(verbosity.c_str(), verbosity.size(), severityLevel);
        boost::log::core::get()->set_filter(boost::log::trivial::severity >= severityLevel);
    } else {
        boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);
    }

    BackendType backendType = BackendType::Openc;

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

    if (deviceIndex >= 16) {
        std::cerr << "Bad device number " << deviceIndex << ". Device number should be in range [0-16]" << "\n";
        return ErrorCode::BadDeviceIndex;
    }

    uint64_t version;
    if (vm.contains("api")) {
        version = vm["api"] .as<uint64_t>();
    } else {
        version = RIF_API_VERSION;
    }

    boost::property_tree::ptree jsonRoot;

    BOOST_LOG_TRIVIAL(debug) << "Create riff context " << to_string(backendType);
    riff::Context context(backendType, deviceIndex, "", version);
    BOOST_LOG_TRIVIAL(debug) << "OK";

    BOOST_LOG_TRIVIAL(debug) << "Get device vendor " << to_string(backendType) << " for gpu" << deviceIndex;
    std::string deviceVendor = getDeviceVendor(backendType, static_cast<int>(deviceIndex));
    BOOST_LOG_TRIVIAL(debug) << "OK";

    jsonRoot.put("vendor", deviceVendor);

    BOOST_LOG_TRIVIAL(debug) << "Get device name " << to_string(backendType) << " for gpu" << deviceIndex;
    std::string deviceName = getDeviceName(backendType, static_cast<int>(deviceIndex));
    BOOST_LOG_TRIVIAL(debug) << "OK";

    jsonRoot.put("name", deviceName);

    BOOST_LOG_TRIVIAL(debug) << "Get device memory size " << to_string(backendType) << " for gpu" << deviceIndex;
    unsigned int deviceMemorySize = getDeviceMemorySize(backendType, static_cast<int>(deviceIndex));
    BOOST_LOG_TRIVIAL(debug) << "OK";

    jsonRoot.put("deviceMemorySize", deviceMemorySize);
    jsonRoot.put("deviceIndex", deviceIndex);

    jsonRoot.put("status", 0);

    boost::property_tree::write_json(std::cout, jsonRoot);
    return 0;
}
catch (const std::exception& error)
{
    boost::property_tree::ptree jsonRoot;
    jsonRoot.put("status", ErrorCode::OtherError);
    jsonRoot.put("errorMessage", error.what());
    boost::property_tree::write_json(std::cout, jsonRoot);

    return ErrorCode::OtherError;
}
catch (...)
{
    boost::property_tree::ptree jsonRoot;
    jsonRoot.put("status", ErrorCode::OtherError);
    jsonRoot.put("errorMessage", "Unknown error");
    boost::property_tree::write_json(std::cout, jsonRoot);


    std::cerr << "Unkown error\n";
    return ErrorCode::UnkownError;
}

