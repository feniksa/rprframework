#include "rprf/Plugin.h"
#include "rprf/Context.h"
#include "rprf/ContextUtils.h"
#include "rprf/Scene.h"
#include "rprf/Error.h"

#include "rprf/Camera.h"
#include "rprf/FrameBuffer.h"
#include "rprf/Shape.h"
#include "rprf/LightPoint.h"
#include "math/mathutils.h"

#include "CubeData.h"

#include <boost/program_options.hpp>
#include <iostream>
#include <filesystem>
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
   s << to_string(rprf::Plugin::Type::HybridPro) << ", ";
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
    using namespace utils;

    bool probeName;
    std::string renderName;
    std::vector<unsigned int> gpuIndexes;
    bool enableCPU;
    unsigned int apiVersion;
    bool renderCube;
    std::string outfile;
    std::string shadercache;
    std::string hipbin;
    unsigned int iterations;
    bool forceOpenCL;

    std::filesystem::path currentDirectory = std::filesystem::path(argv[0]).remove_filename();
    std::filesystem::path hipKernelsDirectory = currentDirectory / std::filesystem::path("hipbin");

    // Declare the supported options.
    po::options_description desc("Generic options");
    desc.add_options()
            ("help", "show help message")
            ("engine", po::value<std::string>(&renderName)->default_value("northstar"),("renderer from list " + allRenderEngines()).c_str())
            ("gpu", po::value<std::vector<unsigned int>>(&gpuIndexes), "gpu indexes to probe")
            ("cpu", po::value<bool>(&enableCPU)->default_value(false), "use cpu")
            ("name", po::value<bool>(&probeName)->default_value(true), "get gpu name from context")
            ("verbosity", po::value<std::string>(), "verbosity")
            ("renderCube", po::value<bool>(&renderCube)->default_value(false), "make actual render of cube")
            ("iterations", po::value<unsigned int>(&iterations)->default_value(100), "how many iterations to render (only if renderCube is 1)")
            ("shadercache", po::value<std::string>(&shadercache)->default_value(""), "directory to store shader cache")
            ("hipbin", po::value<std::string>(&hipbin)->default_value(hipKernelsDirectory.string()), "directory for search precompiled hip kernels")
            ("forceOpenCL", po::value<bool>(&forceOpenCL)->default_value(false), "force enable OpenCL (deprecated)")
            ("out", po::value<std::string>(&outfile), "output filename (if renderCube is true)")
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
    if (forceOpenCL) {
        createFlags |= RPR_CREATION_FLAGS_ENABLE_OPENCL;
    }

    BOOST_LOG_TRIVIAL(debug) << "Initialize library " << to_string(renderer);

    Plugin plugin(renderer);
    BOOST_LOG_TRIVIAL(debug) << "OK";

    BOOST_LOG_TRIVIAL(debug) << "Create context on: " << gpus_as_string(createFlags);
    rprf::Context context(plugin, shadercache, hipbin, createFlags, apiVersion);
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

    if (renderCube) {
        BOOST_LOG_TRIVIAL(debug) << "Construct cube resource for render";
        rprf::Scene scene(context);
        context.setScene(scene);

        // camera
        rprf::Camera camera(context);
        camera.lookAt(
                0, 5, 10,
                0, 0, 0,
                0, 1, 0);
        scene.setCamera(camera);

        rprf::Shape cube(context,
                   reinterpret_cast<rpr_float const *>(&cube_data[0]), 24, sizeof(vertex),
                   reinterpret_cast<rpr_float const *>((char *) &cube_data[0] + sizeof(rpr_float) * 3), 24,
                   sizeof(vertex),
                   reinterpret_cast<rpr_float const *>((char *) &cube_data[0] + sizeof(rpr_float) * 6), 24,
                   sizeof(vertex),
                   static_cast<rpr_int const *>(indices), sizeof(rpr_int),
                   static_cast<rpr_int const *>(indices), sizeof(rpr_int),
                   static_cast<rpr_int const *>(indices), sizeof(rpr_int),
                   num_face_vertices, 12);

        cube.setTransform(rprf_math::translation(rprf_math::float3(-2, 1, 0)), true);
        scene.attachShape(cube);

        // light
        rprf::LightPoint pointLight(context);
        pointLight.setTransform(rprf_math::translation(rprf_math::float3(2, 10, 2)), true);
        pointLight.setRadianPower(150.0f, 150.0f, 150.0f);
        scene.attachLight(pointLight);

        // framebuffer
        rprf::FrameBuffer frameBuffer(context, 800, 600);
        rprf::FrameBuffer frameBufferResolved(frameBuffer.clone());

        context.setAOV(frameBuffer);
        context.setParameter1u(ContextInputType::Iterations, iterations);

        BOOST_LOG_TRIVIAL(debug) << "Render cube";
        context.render();
        BOOST_LOG_TRIVIAL(debug) << "Rendering done";

        BOOST_LOG_TRIVIAL(debug) << "Resolve framebuffer";
        context.resolve(&frameBuffer, &frameBufferResolved, false);
        BOOST_LOG_TRIVIAL(debug) << "Framebuffer resolved";

        if (!outfile.empty()) {
            BOOST_LOG_TRIVIAL(debug) << "Save framebuffer to file " << outfile;
            frameBufferResolved.saveToFile(outfile);
            BOOST_LOG_TRIVIAL(debug) << "Save done";
        }
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
