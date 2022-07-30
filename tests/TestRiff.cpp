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
	auto devices = getAvailableDevices(BackendType::Openc);
	std::cout << "Available riff devices: " << devices.size() << "\n";
	ASSERT_TRUE(devices.size() > 0);

	printAvailableDevices(devices, std::cout);
}
