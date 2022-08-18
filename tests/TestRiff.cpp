#include "Environment.h"

#include "riff/Context.h"
#include "riff/ContextUtils.h"

#include <gtest/gtest.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "ImageTools/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "ImageTools/stb_image.h"

#include "ImageTools/ImageTools.h"

using namespace riff;

struct TestRiff : public ::testing::Test
{
    void SetUp()
    {
    }

    void TearDown()
    {
    }
};

TEST_F(TestRiff, create_context)
{
	auto devices = getAvailableDevices(BackendType::Openc);
	std::cout << "Available riff devices: " << devices.size() << "\n";
	ASSERT_TRUE(devices.size() > 0);

	printAvailableDevices(devices, std::cout);

    Context context(BackendType::Openc, 0);

    rif_image normalsImage = ImageTools::LoadImage((tests::ResourcesDirectory / "Images/normal.exr").string().c_str(), context);
    rif_image vNormalsImage = ImageTools::LoadImage("images/v_normal.exr", context);
    rif_image depthImage = ImageTools::LoadImage("images/depth.exr", context);
    rif_image vDepthImage = ImageTools::LoadImage("images/v_depth.exr", context);
    rif_image colorImage = ImageTools::LoadImage("images/color.exr", context);
    rif_image vColorImage = ImageTools::LoadImage("images/v_color.exr", context);
    rif_image transitionImage = ImageTools::LoadImage("images/transition.exr", context);
    rif_image vTransitionImage = ImageTools::LoadImage("images/v_transition.exr", context);
    rif_image outputImage = nullptr;

   ASSERT_TRUE(normalsImage);

}
