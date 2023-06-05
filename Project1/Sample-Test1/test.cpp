#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../Project1/DeviceDriver.cpp"

using namespace testing;
using namespace std;

const long TEST_ADDR[] = {0x00, 0x01, 0x02, 0x03, 0x04};

class MockFlashM : public FlashMemoryDevice
{
public:
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

class DeviceDriverFixture : public Test
{
public:
	MockFlashM mockFlash;
	DeviceDriver deviceDriver{ &mockFlash };
};

TEST_F(DeviceDriverFixture, ReadFailExceptionCase) {

	EXPECT_CALL(mockFlash, read)
		.WillOnce(Return(0x01))
		.WillRepeatedly(Return(0x00));

	EXPECT_THROW(deviceDriver.read(TEST_ADDR[0]), ReadFailException);
}

TEST_F(DeviceDriverFixture, Read5TimesTest) {

	EXPECT_CALL(mockFlash, read)
		.WillRepeatedly(Return(0x00));

	EXPECT_CALL(mockFlash, read)
		.Times(5);

	deviceDriver.read(TEST_ADDR[0]);
}

TEST_F(DeviceDriverFixture, ReadTimeMeasure200ms4Times) {

	EXPECT_CALL(mockFlash, read)
		.WillRepeatedly(Return(0x00));

	EXPECT_CALL(mockFlash, read)
		.Times(5);

	auto start = chrono::steady_clock::now();

	deviceDriver.read(TEST_ADDR[0]);

	auto end = chrono::steady_clock::now();
	chrono::duration<double> elapsed = end - start;

	EXPECT_GE(elapsed.count(), 0.8);
	EXPECT_LE(elapsed.count(), 1.0);
}

TEST_F(DeviceDriverFixture,WriteFailExceptionTest)
{
	EXPECT_CALL(mockFlash, read)
		.WillRepeatedly(Return(0xFF));
	EXPECT_THROW(deviceDriver.write(TEST_ADDR[0], 0), WriteFailException);
}

TEST_F(DeviceDriverFixture, SingleWriteDoneTest)
{
	EXPECT_CALL(mockFlash, read)
		.WillRepeatedly(Return(0x00));

	EXPECT_CALL(mockFlash, read)
		.Times(1);

	deviceDriver.write(TEST_ADDR[0], 3);
}

TEST_F(DeviceDriverFixture, WriteAllDoneTest)
{
	EXPECT_CALL(mockFlash, read)
		.WillRepeatedly(Return(0x00));

	EXPECT_CALL(mockFlash, read)
		.Times(5);

	for (long address : TEST_ADDR) {
		deviceDriver.write(address, 3);
	}
}