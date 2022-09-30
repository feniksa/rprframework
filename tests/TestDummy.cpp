#include <gtest/gtest.h>

struct TestDummy : public ::testing::Test
{
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(TestDummy, empty)
{
}
