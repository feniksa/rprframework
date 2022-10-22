#include "rprf/Plugin.h"
#include "rprf/Context.h"
#include "rprf/ContextUtils.h"

#include <boost/program_options.hpp>
#include <iostream>
#include <boost/log/trivial.hpp>

namespace po = boost::program_options;
using namespace rprf;

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

std::string allRenderEngines()
{
   std::stringstream s;

   s << to_string(rprf::Plugin::Type::Northstar) << ", ";
   s << to_string(rprf::Plugin::Type::Hybrid)    << ", ";
   s << to_string(rprf::Plugin::Type::Tahoe)     << "  ";

   return s.str();
}

} // end of namespace

int main(int argc, const char **argv) try
{
    // Declare the supported options.
    po::options_description desc("Generic options");
    desc.add_options()
            ("help", "show help message")
            ("engine", po::value<std::string>()->default_value("northstar"),("renderer from list " + allRenderEngines()).c_str())
            ("gpu", po::value<std::vector<unsigned int>>(), "gpu indexes to probe")
            ("cpu", po::value<bool>()->default_value(true), "use cpu cpu")
            ("api", po::value<int>()->default_value(RPR_API_VERSION), "force to use API version");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.contains("help")) {
        std::cout << desc << "\n";
        return ErrorCode::Ok;
    }

    Plugin::Type renderer;
    unsigned int gpuDeviceIndex;
    int createFlags = 0;

    // parse requirements for engine
    if (!vm.contains("engine")) {
        std::cerr << "Not enough params";
        return NotEnoughParams;
    }

    std::string renderName = vm["engine"].as<std::string>();

    // to lower case
    std::transform(renderName.begin(), renderName.end(), renderName.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    // parse string from cmd for render type: tahoe or northstar or hybrid
    bool result;
    std::tie(renderer, result) = rprf::from_string(renderName);
    if (!result) { // bad string
        std::cerr << "Unknown renderer engine. Supported renders are:" << allRenderEngines() << "\n";
        return ErrorCode::BadCompute;
    }

    if (vm.contains("gpu")) {
        const auto gpuIndexes = vm["gpu"].as<std::vector<unsigned int>>();
        for (unsigned int index : gpuIndexes) {
            if (index >= 16) {
                std::cerr << "Bad gpu number " <<  index << ". GPU number should be in range [0-16]" << "\n";
                return ErrorCode::BadDeviceIndex;
            }
            createFlags |= getContextCreationFlags(index);
        }
    }

    if (vm.contains("cpu")) {
        bool enableCPU = vm["cpu"].as<bool>();
        if (enableCPU) {
            createFlags |= RPR_CREATION_FLAGS_ENABLE_CPU;
        }
    }

    unsigned int version;
    if (vm.contains("api")) {
        version = vm["api"] .as<int>();
    } else {
        version = RPR_API_VERSION;
    }

    BOOST_LOG_TRIVIAL(debug) << "Initialize library " << to_string(renderer);

    Plugin plugin(renderer);
    BOOST_LOG_TRIVIAL(debug) << "OK";

    BOOST_LOG_TRIVIAL(debug) << "Create context";
    rprf::Context context(plugin, "", createFlags, version);
    BOOST_LOG_TRIVIAL(debug) << "OK";

    return 0;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << "\n";
    return ErrorCode::OtherError;
}
catch (...) {
    std::cerr << "Unkown error\n";
    return ErrorCode::UnkownError;
}
