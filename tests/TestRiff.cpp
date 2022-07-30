#include "riff/Context.h"
#include "riff/ContextUtils.h"

#include <gtest/gtest.h>

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
	int devices = getDeviceCount(BackendType::Openc);
	std::cout << "Available riff devices: " << devices << "\n";
	ASSERT_TRUE(devices > 0);

	printAvailableDevices(BackendType::Openc, std::cout);
}
