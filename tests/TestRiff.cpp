#include "Environment.h"

#include "riff/Context.h"
#include "riff/ContextUtils.h"
#include "riff/ContextQueue.h"
#include "riff/Image.h"
#include "riff/ImageFilter.h"

#include <gtest/gtest.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "ImageTools/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "ImageTools/stb_image.h"

#include "ImageTools/ImageTools.h"

#include <sstream>

using namespace riff;

class RifImageTest : public riff::ContextObject<rif_image>
{
public:
    explicit RifImageTest(riff::Context& context, const std::filesystem::path& path)
    {
        rif_image image = ImageTools::LoadImage(path, context);
        if (!image) {
            throw std::runtime_error("Can't load image \"" + path.string() + "\"");
        }
        setInstance(std::move(image));
    }
private:
};

struct TestRiff : public ::testing::Test
{
    void SetUp()
    {
    }

    void TearDown()
    {
    }
};

TEST_F(TestRiff, denoiser)
{
	auto devices = getAvailableDevices(BackendType::Openc);
	std::cout << "Available riff devices: " << devices.size() << "\n";
	ASSERT_TRUE(devices.size() > 0);

	printAvailableDevices(devices, std::cout);

    Context context(BackendType::Openc, devices[0].deviceId);

    Image normalsImage(context, tests::ResourcesDirectory / "Images/normal.exr");
    Image vNormalsImage(context, tests::ResourcesDirectory / "images/v_normal.exr");
    Image depthImage(context, tests::ResourcesDirectory / "images/depth.exr");
    Image vDepthImage(context, tests::ResourcesDirectory / "images/v_depth.exr");
    Image colorImage(context, tests::ResourcesDirectory / "images/color.exr");
    Image vColorImage(context, tests::ResourcesDirectory / "images/v_color.exr");
    Image transitionImage(context, tests::ResourcesDirectory / "images/transition.exr");
    Image vTransitionImage(context, tests::ResourcesDirectory / "images/v_transition.exr");

    std::unique_ptr<Image> outputImage;

    ASSERT_TRUE(normalsImage);
    ASSERT_TRUE(vNormalsImage);
    ASSERT_TRUE(depthImage);
    ASSERT_TRUE(vDepthImage);
    ASSERT_TRUE(colorImage);
    ASSERT_TRUE(vColorImage);
    ASSERT_TRUE(transitionImage);
    ASSERT_TRUE(vTransitionImage);
    ASSERT_FALSE(outputImage);

    // test load functionality for exr image

    std::vector<rif_image> inputs = { colorImage, normalsImage, depthImage, transitionImage };
    std::vector<float> sigmas = { 0.01f, 0.01f, 0.01f, 0.01f };

    ContextQueue queue(context);

    ImageDescription desc = colorImage.getImageInfo();
    desc.type = RIF_COMPONENT_TYPE_UINT8;
    outputImage = std::make_unique<Image>(context, desc);

    // denoise Bilateral
    ImageFilter bilateralFilter(context, ImageFilterType::BilateralDenoise);
    bilateralFilter.setParameterImageArray("inputs", inputs);
    bilateralFilter.setParameterFloatArray("sigmas", sigmas);
    bilateralFilter.setParameter1u("radius", 10);
    bilateralFilter.setParameter1u("inputsNum", inputs.size());

    queue.attachFilter(bilateralFilter, colorImage, outputImage.get());

    queue.execute();

    outputImage->save("c:/temp/test.png");

}
