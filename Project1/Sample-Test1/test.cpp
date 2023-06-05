#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../Project1/DeviceDriver.cpp"

using namespace testing;
using namespace std;

const long TEST_ADDR_1 = 0x00;
const long TEST_ADDR_2 = 0x01;
const long TEST_ADDR_3 = 0x02;
const long TEST_ADDR_4 = 0x03;
const long TEST_ADDR_5 = 0x04;

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
	long readAddr;
};

TEST_F(DeviceDriverFixture, ReadFailExceptionCase) {

	readAddr = TEST_ADDR_1;
	EXPECT_CALL(mockFlash, read)
		.WillOnce(Return('1'))
		.WillRepeatedly(Return('0'));

	EXPECT_THROW(deviceDriver.read(readAddr), ReadFailException);
}

TEST_F(DeviceDriverFixture, Read5TimesTest) {

	readAddr = TEST_ADDR_1;
	EXPECT_CALL(mockFlash, read)
		.WillRepeatedly(Return('0'));

	EXPECT_CALL(mockFlash, read)
		.Times(5);

	deviceDriver.read(readAddr);
}

TEST_F(DeviceDriverFixture, ReadTimeMeasure200ms4Times) {

	readAddr = TEST_ADDR_1;
	EXPECT_CALL(mockFlash, read)
		.WillRepeatedly(Return('0'));

	EXPECT_CALL(mockFlash, read)
		.Times(5);

	auto start = chrono::steady_clock::now();

	deviceDriver.read(readAddr);

	auto end = chrono::steady_clock::now();
	chrono::duration<double> elapsed = end - start;

	EXPECT_GE(elapsed.count(), 0.8);
	EXPECT_LE(elapsed.count(), 1.0);
}