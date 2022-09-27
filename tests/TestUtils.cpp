#include <gtest/gtest.h>

#include "Environment.h"
#include "Utils.h"

struct TestUtils : public ::testing::Test
{
    void SetUp()
    {
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
