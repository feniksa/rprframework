#include "Renderer.h"

#include "rprf/Plugin.h"
#include "rprf/Context.h"
#include "rprf/ContextUtils.h"
#include "rprf/Camera.h"

#include <QDir>

Renderer::Renderer(std::filesystem::path hipbin)
{
    QDir kernelsDir = hipbin;

    //std::string hipbinPath("hipbin");      

    /*auto plugin = std::make_unique<rprf::Plugin>(rprf::Plugin::Type::Northstar);
    const auto gpus = getAvailableDevices(*plugin, "", hipbinPath);

    int creationFlags = GetCreationFlags(gpus);
    rprf::Context context(*plugin, "", hipbinPath, creationFlags);

    const auto transform = rprf_math::matrix(
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        9.0f, 10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
        );

    rprf::Camera camera(context);

    camera.setTransform(transform, false);

    const rprf_math::matrix m = camera.getTransform();*/

}

std::filesystem::path Renderer::getHipBin()
{
    return QDir::home().absoluteFilePath(".hipbin").toStdString();
}
