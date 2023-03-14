#include "rprf/Plugin.h"
#include "rprf/Context.h"
#include "rprf/ContextUtils.h"

#include <boost/program_options.hpp>
#include <iostream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace po = boost::program_options;
using namespace rprf;

enum ErrorCode {
    Ok = 0,
    NotEnoughParams = -1,
    BadDevice = -2,
    BadDeviceIndex = -3,
    OtherError = -4,
    UnkownError = -5,
};

namespace
{

    const char* GenericComputeDeviceName = "Generic Compute Device";

std::string allRenderEngines()
{
   std::stringstream s;

   s << to_string(rprf::Plugin::Type::Northstar) << ", ";
   s << to_string(rprf::Plugin::Type::Hybrid)    << ", ";
   s << to_string(rprf::Plugin::Type::HybridPro)    << ", ";
   s << to_string(rprf::Plugin::Type::Tahoe)     << "  ";

   return s.str();
}

std::string gpus_as_string(int creationFlags)
{
    std::ostringstream s;

    if (creationFlags & RPR_CREATION_FLAGS_ENABLE_CPU) s << "CPU ";
    if (creationFlags & RPR_CREATION_FLAGS_ENABLE_GPU0) s << "GPU0 ";
    if (creationFlags & RPR_CREATION_FLAGS_ENABLE_GPU1) s << "GPU1 ";
    if (creationFlags & RPR_CREATION_FLAGS_ENABLE_GPU2) s << "GPU2 ";
    if (creationFlags & RPR_CREATION_FLAGS_ENABLE_GPU3) s << "GPU3 ";
    if (creationFlags & RPR_CREATION_FLAGS_ENABLE_GPU4) s << "GPU4 ";
    if (creationFlags & RPR_CREATION_FLAGS_ENABLE_GPU5) s << "GPU5 ";
    if (creationFlags & RPR_CREATION_FLAGS_ENABLE_GPU6) s << "GPU6 ";
    if (creationFlags & RPR_CREATION_FLAGS_ENABLE_GPU7) s << "GPU7 ";
    if (creationFlags & RPR_CREATION_FLAGS_ENABLE_GPU8) s << "GPU8 ";
    if (creationFlags & RPR_CREATION_FLAGS_ENABLE_GPU9) s << "GPU9 ";
    if (creationFlags & RPR_CREATION_FLAGS_ENABLE_GPU10) s << "GPU10 ";
    if (creationFlags & RPR_CREATION_FLAGS_ENABLE_GPU11) s << "GPU11 ";
    if (creationFlags & RPR_CREATION_FLAGS_ENABLE_GPU12) s << "GPU12 ";
    if (creationFlags & RPR_CREATION_FLAGS_ENABLE_GPU13) s << "GPU13 ";
    if (creationFlags & RPR_CREATION_FLAGS_ENABLE_GPU14) s << "GPU14 ";
    if (creationFlags & RPR_CREATION_FLAGS_ENABLE_GPU15) s << "GPU15 ";

    return s.str();
}

} // end of namespace

int main(int argc, const char **argv) try
{
    bool probeName;
    std::string renderName;
    std::vector<unsigned int> gpuIndexes;
    bool enableCPU;
    unsigned int apiVersion;

    // Declare the supported options.
    po::options_description desc("Generic options");
    desc.add_options()
            ("help", "show help message")
            ("engine", po::value<std::string>(&renderName)->default_value("northstar"),("renderer from list " + allRenderEngines()).c_str())
            ("gpu", po::value<std::vector<unsigned int>>(&gpuIndexes), "gpu indexes to probe")
            ("cpu", po::value<bool>(&enableCPU)->default_value(false), "use cpu")
            ("name", po::value<bool>(&probeName)->default_value(true), "get gpu name from context")
            ("verbosity", po::value<std::string>(), "verbosity")
            ("api", po::value<unsigned int>(&apiVersion)->default_value(RPR_API_VERSION), "force to use API version");

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

    Plugin::Type renderer;
    int createFlags = 0;

    // to lower case
    std::transform(renderName.begin(), renderName.end(), renderName.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    // parse string from cmd for render type: tahoe or northstar or hybrid
    bool result;
    std::tie(renderer, result) = rprf::from_string(renderName);
    if (!result) { // bad string
        std::cerr << "Unknown renderer engine. Supported renders are:" << allRenderEngines() << "\n";
        return ErrorCode::BadDevice;
    }

    for (unsigned int index : gpuIndexes) {
        if (index >= 16) {
            std::cerr << "Bad gpu number " <<  index << ". GPU number should be in range [0-16]" << "\n";
            return ErrorCode::BadDeviceIndex;
        }
        createFlags |= getContextCreationFlags(index);
    }

    if (enableCPU) {
        createFlags |= RPR_CREATION_FLAGS_ENABLE_CPU;
    }

    BOOST_LOG_TRIVIAL(debug) << "Initialize library " << to_string(renderer);

    Plugin plugin(renderer);
    BOOST_LOG_TRIVIAL(debug) << "OK";

    BOOST_LOG_TRIVIAL(debug) << "Create context on: " << gpus_as_string(createFlags);
    rprf::Context context(plugin, "", createFlags, apiVersion);
    BOOST_LOG_TRIVIAL(debug) << "OK";

    boost::property_tree::ptree jsonRoot;

    if (createFlags & RPR_CREATION_FLAGS_ENABLE_CPU) {
        boost::property_tree::ptree jsonChild;

        BOOST_LOG_TRIVIAL(debug) << "Get CPU name: ";
        std::string cpuName = context.getCpuName();
        BOOST_LOG_TRIVIAL(debug) << "OK";

        jsonChild.put(std::to_string(0), cpuName);

        jsonRoot.add_child("cpus", jsonChild);
    }

    for (unsigned int gpuIndex : gpuIndexes) {
        boost::property_tree::ptree jsonChild;
        std::string gpuName;

        if (probeName) {
            BOOST_LOG_TRIVIAL(debug) << "Get GPU name: " << gpuIndex;
            gpuName = context.getGpuName(gpuIndex);
            BOOST_LOG_TRIVIAL(debug) << "OK";
        } else {
            gpuName = GenericComputeDeviceName;
        }

        jsonChild.put(std::to_string(gpuIndex), gpuName);
        jsonRoot.add_child("gpus", jsonChild);
    }

    jsonRoot.put("status", 0);

    boost::property_tree::write_json(std::cout, jsonRoot);

    return 0;
}
catch (const Error& error)
{
    boost::property_tree::ptree jsonRoot;
    jsonRoot.put("status", ErrorCode::BadDevice);
    jsonRoot.put("errorMessage", error.what());
    boost::property_tree::write_json(std::cout, jsonRoot);

    return ErrorCode::BadDevice;
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
    jsonRoot.put("status", ErrorCode::UnkownError);
    jsonRoot.put("errorMessage", "Unknown error");
    boost::property_tree::write_json(std::cout, jsonRoot);

    return ErrorCode::UnkownError;
}
