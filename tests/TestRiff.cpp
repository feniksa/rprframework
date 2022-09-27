#include "Environment.h"

#include "riff/Context.h"
#include "riff/ContextUtils.h"
#include "riff/ContextQueue.h"
#include "riff/Image.h"
#include "riff/ImageFilter.h"

#include <gtest/gtest.h>
#include "utils.h"

#include <sstream>

using namespace riff;

struct TestRiff : public ::testing::Test
{
    std::filesystem::path m_tempDir;

    TestRiff() {
        m_tempDir = std::filesystem::temp_directory_path();
        m_tempDir /= "RadeonProRenderTests";
        if (!std::filesystem::exists(m_tempDir)) {
            std::filesystem::create_directory(m_tempDir);
        }

        m_tempDir /= "TestRiff";
        if (!std::filesystem::exists(m_tempDir)) {
            std::filesystem::create_directory(m_tempDir);
        }
        std::cout << "Temporary directory: \t" << m_tempDir <<  "\n";
    }

    void SetUp()
    {
    }

    void TearDown()
    {
    }
};

TEST_F(TestRiff, denoiser_bilaterial)
{
	auto devices = getAvailableDevices(BackendType::Openc);
	std::cout << "Available riff devices: " << devices.size() << "\n";
	ASSERT_TRUE(devices.size() > 0);

	printAvailableDevices(devices, std::cout);

    Context context(BackendType::Openc, devices[0].deviceId);

    Image normalsImage(context, tests::ResourcesDirectory / "images/normal.exr");
    Image depthImage(context, tests::ResourcesDirectory / "images/depth.exr");
    Image colorImage(context, tests::ResourcesDirectory / "images/color.exr");
    Image transitionImage(context, tests::ResourcesDirectory / "images/transition.exr");
    Image outputImage(context);

    ASSERT_TRUE(normalsImage);
    ASSERT_TRUE(depthImage);
    ASSERT_TRUE(colorImage);
    ASSERT_TRUE(transitionImage);
    ASSERT_FALSE(outputImage);

    // test load functionality for exr image

    std::vector<rif_image> inputs = { colorImage, normalsImage, depthImage, transitionImage };
    std::vector<float> sigmas = { 0.01f, 0.01f, 0.01f, 0.01f };

    ContextQueue queue(context);

    ImageDescription desc = colorImage.getImageInfo();
    desc.type = RIF_COMPONENT_TYPE_UINT8;
    outputImage.allocate(desc);

    // denoise Bilateral
    ImageFilter bilateralFilter(context, ImageFilterType::BilateralDenoise);
    bilateralFilter.setParameterImageArray("inputs", inputs);
    bilateralFilter.setParameterFloatArray("sigmas", sigmas);
    bilateralFilter.setParameter1u("radius", 10);
    bilateralFilter.setParameter1u("inputsNum", inputs.size());

    queue.attachFilter(&bilateralFilter, &colorImage, &outputImage);
    queue.execute();

    outputImage.saveToFile(m_tempDir / "denoiser_bilaterial.png");
}

TEST_F(TestRiff, denoiser_lwr)
{
    auto devices = getAvailableDevices(BackendType::Openc);
    std::cout << "Available riff devices: " << devices.size() << "\n";
    ASSERT_TRUE(devices.size() > 0);

    Context context(BackendType::Openc, devices[0].deviceId);

    Image normalsImage(context, tests::ResourcesDirectory / "images" / "normal.exr");
    Image vNormalsImage(context, tests::ResourcesDirectory / "images" / "v_normal.exr");
    Image depthImage(context, tests::ResourcesDirectory / "images" / "depth.exr");
    Image vDepthImage(context, tests::ResourcesDirectory / "images" / "v_depth.exr");
    Image colorImage(context, tests::ResourcesDirectory / "images" / "color.exr");
    Image vColorImage(context, tests::ResourcesDirectory / "images" / "v_color.exr");
    Image transitionImage(context, tests::ResourcesDirectory / "images" / "transition.exr");
    Image vTransitionImage(context, tests::ResourcesDirectory / "images" / "v_transition.exr");

    Image outputImage(context);

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
    outputImage.allocate(desc);

    // denoise Bilateral
    ImageFilter lwrDenoise(context, ImageFilterType::LWR_Denoise);
    lwrDenoise.setParameterImage("vColorImg", vColorImage);
    lwrDenoise.setParameterImage("transImg", transitionImage);
    lwrDenoise.setParameterImage("vTransImg", vTransitionImage);
    lwrDenoise.setParameterImage("normalsImg", normalsImage);
    lwrDenoise.setParameterImage("depthImg", depthImage);
    lwrDenoise.setParameterImage("vDepthImg", vDepthImage);

    queue.attachFilter(&lwrDenoise, &colorImage, &outputImage);
    queue.execute();

    outputImage.saveToFile(m_tempDir / "denoiser_lwr.png");
}

TEST_F(TestRiff, denoiser_eaw)
{
    auto devices = getAvailableDevices(BackendType::Openc);
    std::cout << "Available riff devices: " << devices.size() << "\n";
    ASSERT_TRUE(devices.size() > 0);

    Context context(BackendType::Openc, devices[0].deviceId);

    Image normalsImage(context, tests::ResourcesDirectory / "images" / "normal.exr");
    Image vNormalsImage(context, tests::ResourcesDirectory / "images" / "v_normal.exr");
    Image depthImage(context, tests::ResourcesDirectory / "images" / "depth.exr");
    Image vDepthImage(context, tests::ResourcesDirectory / "images" / "v_depth.exr");
    Image colorImage(context, tests::ResourcesDirectory / "images" / "color.exr");
    Image vColorImage(context, tests::ResourcesDirectory / "images" /"v_color.exr");
    Image transitionImage(context, tests::ResourcesDirectory / "images" / "transition.exr");
    Image vTransitionImage(context, tests::ResourcesDirectory / "images" / "v_transition.exr");

    Image outputImage(context);

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
    outputImage.allocate(desc);

    // denoise Bilateral
    ImageFilter lwrDenoise(context, ImageFilterType::EawDenoise);
    lwrDenoise.setParameterImage("colorVar", vColorImage);
    lwrDenoise.setParameterImage("transImg", transitionImage);
    lwrDenoise.setParameterImage("normalsImg", normalsImage);
    lwrDenoise.setParameterImage("depthImg", depthImage);


    queue.attachFilter(&lwrDenoise, &colorImage, &outputImage);
    queue.execute();

    outputImage.saveToFile(m_tempDir / "denoiser_eaw.png");
}

TEST_F(TestRiff, custom_filter)
{
    const rif_char code[] =
            "int2 coord;"
            "int2 size = GET_BUFFER_SIZE(outputImage);"
            "GET_COORD_OR_RETURN(coord, size);"
            "vec4 pixel = ReadPixelTyped(inputImage, coord.x, coord.y);"
            "vec2 dxy = convert_vec2(coord)/convert_vec2(size) - (vec2)0.5f;"
            "pixel = mix(1.0f, pixel, exp(-dot(dxy, dxy) * 5));"
            "WritePixelTyped(outputImage, coord.x, coord.y, pixel);";

    auto devices = getAvailableDevices(BackendType::Openc);
    std::cout << "Available riff devices: " << devices.size() << "\n";
    ASSERT_TRUE(devices.size() > 0);

    Context context(BackendType::Openc, devices[0].deviceId);

    Image inputImage(context, tests::ResourcesDirectory / "images" / "lenna.png");

    Image outputImage(context);

    ASSERT_TRUE(inputImage);
    ASSERT_FALSE(outputImage);

    ContextQueue queue(context);

    ImageDescription desc = inputImage.getImageInfo();
    outputImage.allocate(desc);

    ImageFilter userDefined(context, ImageFilterType::UserDefined);
    userDefined.setParameterString("code", code);

    queue.attachFilter(&userDefined, &inputImage, &outputImage);
    queue.execute();
    queue.detachFilter(&userDefined);

    outputImage.saveToFile(m_tempDir / "user_defined.png");
}

TEST_F(TestRiff, image_memory_copy)
{
    const rif_char code[] =
            "int2 coord;"
            "int2 size = GET_BUFFER_SIZE(outputImage);"
            "GET_COORD_OR_RETURN(coord, size);"
            "vec4 pixel = ReadPixelTyped(inputImage, coord.x, coord.y);"
            "WritePixelTyped(outputImage, coord.x, coord.y, pixel);";

    auto devices = getAvailableDevices(BackendType::Openc);
    ASSERT_TRUE(devices.size() > 0);

    Context context(BackendType::Openc, devices[0].deviceId);

    Image inputImage1(context, tests::ResourcesDirectory / "images" / "lenna.png");
    Image inputImage2 = inputImage1.copy();

    Image outputImage(context);

    ASSERT_TRUE(inputImage1);
    ASSERT_TRUE(inputImage2);
    ASSERT_FALSE(outputImage);

    EXPECT_EQ(inputImage1.getImageInfo().image_height, inputImage2.getImageInfo().image_height);
    EXPECT_EQ(inputImage1.getImageInfo().image_width, inputImage2.getImageInfo().image_width);
    EXPECT_EQ(inputImage1.getImageInfo().num_components, inputImage2.getImageInfo().num_components);
    EXPECT_EQ(inputImage1.getImageInfo().type, inputImage2.getImageInfo().type);

    ContextQueue queue(context);

    ImageDescription desc = inputImage1.getImageInfo();
    outputImage.allocate(desc);

    ImageFilter userDefined(context, ImageFilterType::UserDefined);
    userDefined.setParameterString("code", code);

    queue.attachFilter(&userDefined, &inputImage1, &outputImage);

    queue.execute();
    queue.synchronize();
    queue.flush();

    queue.detachFilter(&userDefined);

    outputImage.saveToFile(m_tempDir / "image_memory_copy.png");

    EXPECT_TRUE(images_same(tests::ResourcesDirectory / "images" / "lenna.png", m_tempDir / "image_memory_copy.png"));
}

TEST_F(TestRiff, image_compare_same)
{
    const rif_char code[] =
            "int2 coord;"
            "int2 size = GET_BUFFER_SIZE(outputImage);"
            "GET_COORD_OR_RETURN(coord, size);"
            "vec4 pixel1 = ReadPixelTyped(inputImage, coord.x, coord.y);"
            "vec4 pixel2 = ReadPixelTyped(inputImage2, coord.x, coord.y);"
            /*"vec4 pixel = make_vec4(abs_diff(pixel1.x, pixel2.x), abs_diff(pixel1.x, pixel2.x), abs_diff(pixel1.x, pixel2.x));"*/
            "vec4 pixel = make_vec4(fabs(pixel1.x - pixel2.x), fabs(pixel1.y - pixel2.y), fabs(pixel1.z - pixel2.z), fabs(pixel1.w - pixel2.w));"
            "WritePixelTyped(outputImage, coord.x, coord.y, pixel);";

    auto devices = getAvailableDevices(BackendType::Openc);
    std::cout << "Available riff devices: " << devices.size() << "\n";
    ASSERT_TRUE(devices.size() > 0);

    printAvailableDevices(devices, std::cout);

    Context context(BackendType::Openc, devices[0].deviceId);

    Image inputImage1(context, tests::ResourcesDirectory / "images" / "lenna.png");
    Image inputImage2 = inputImage1.copy();
    Image inputImage3(context, tests::ResourcesDirectory / "images" / "lenna_rotated_180.png");

    Image outputImage(context);

    ASSERT_TRUE(inputImage1);
    ASSERT_TRUE(inputImage2);
    ASSERT_FALSE(outputImage);

    EXPECT_EQ(inputImage1.getImageInfo().image_height, inputImage2.getImageInfo().image_height);
    EXPECT_EQ(inputImage1.getImageInfo().image_width, inputImage2.getImageInfo().image_width);
    EXPECT_EQ(inputImage1.getImageInfo().num_components, inputImage2.getImageInfo().num_components);
    EXPECT_EQ(inputImage1.getImageInfo().type, inputImage2.getImageInfo().type);

    ContextQueue queue(context);

    ImageDescription desc = inputImage1.getImageInfo();
    outputImage.allocate(desc);

    ImageFilter userDefined(context, ImageFilterType::UserDefined);
    userDefined.setParameterString("code", code);
    userDefined.setParameterImage("inputImage2", inputImage2);

    queue.attachFilter(&userDefined, &inputImage1, &outputImage);
    queue.execute();
    outputImage.saveToFile(m_tempDir / "image_diff.png");

    userDefined.setParameterImage("inputImage2", inputImage3);
    queue.execute();
    outputImage.saveToFile(m_tempDir / "image_diff2.png");

}