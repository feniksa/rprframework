#include <gtest/gtest.h>

struct TestDummy : public ::testing::Test
{
    void SetUp()
    {
    }

    void TearDown()
    {
    }
};

TEST_F(TestDummy, empty)
{
}
