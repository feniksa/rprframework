#include <gtest/gtest.h>

#include "Environment.h"
#include "Utils.h"

struct TestUtils : public ::testing::Test
{
    std::filesystem::path m_tempDir;

    void SetUp()
    {
        m_tempDir = std::filesystem::temp_directory_path();
        m_tempDir /= "RadeonProRenderTests";
        if (!std::filesystem::exists(m_tempDir)) {
            std::filesystem::create_directory(m_tempDir);
        }

        m_tempDir /= "TestUtils";
        if (!std::filesystem::exists(m_tempDir)) {
            std::filesystem::create_directory(m_tempDir);
        }
    }

    void TearDown()
    {
    }
};

TEST_F(TestUtils, images_same)
{
    auto ref_image_path = tests::ResourcesDirectory / "images" / "lenna.png";
    EXPECT_TRUE(images_same(ref_image_path, ref_image_path));
}

TEST_F(TestUtils, images_different)
{
    auto ref_image_path = tests::ResourcesDirectory / "images" / "lenna.png";
    auto ref_image2_path = tests::ResourcesDirectory / "images" / "lenna_rotated_180.png";

    EXPECT_FALSE(images_same(ref_image_path, ref_image2_path));
}

TEST_F(TestUtils, load_save_image)
{
    auto ref_image_path = tests::ResourcesDirectory / "images" / "lenna.png";
    MemoryImage image1(ref_image_path.string().c_str());

    auto save_path = ref_image_path / "lenna_saved.png";
    bool result = image1.save(save_path.string().c_str());
    EXPECT_TRUE(result);

    MemoryImage image2;
    result = image2.load(save_path.string().c_str());
    EXPECT_TRUE(result) << "can't reload saved image";

    EXPECT_TRUE(images_same(ref_image_path, save_path));
}
