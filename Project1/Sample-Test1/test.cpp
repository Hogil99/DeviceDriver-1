#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../Project1/DeviceDriver.cpp"
#include "../Project1/App.cpp"

using namespace testing;
using namespace std;

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
	Application app;
};

TEST_F(DeviceDriverFixture, ReadFailExceptionCase) {

	EXPECT_CALL(mockFlash, read)
		.WillOnce(Return(0x01))
		.WillRepeatedly(Return(0x00));

	EXPECT_THROW(deviceDriver.read(ADDR_LIST[0]), ReadFailException);
}

TEST_F(DeviceDriverFixture, Read5TimesTest) {

	EXPECT_CALL(mockFlash, read)
		.WillRepeatedly(Return(0x00));

	EXPECT_CALL(mockFlash, read)
		.Times(5);

	deviceDriver.read(ADDR_LIST[0]);
}

TEST_F(DeviceDriverFixture, ReadTimeMeasure200ms4Times) {

	EXPECT_CALL(mockFlash, read)
		.WillRepeatedly(Return(0x00));

	EXPECT_CALL(mockFlash, read)
		.Times(5);

	auto start = chrono::steady_clock::now();

	deviceDriver.read(ADDR_LIST[0]);

	auto end = chrono::steady_clock::now();
	chrono::duration<double> elapsed = end - start;

	EXPECT_GE(elapsed.count(), 0.8);
	EXPECT_LE(elapsed.count(), 1.0);
}

TEST_F(DeviceDriverFixture,WriteFailExceptionTest)
{
	EXPECT_CALL(mockFlash, read)
		.WillRepeatedly(Return(0xFF));
	EXPECT_THROW(deviceDriver.write(ADDR_LIST[0], 0), WriteFailException);
}

TEST_F(DeviceDriverFixture, SingleWriteDoneTest)
{
	EXPECT_CALL(mockFlash, read)
		.WillRepeatedly(Return(0x00));

	EXPECT_CALL(mockFlash, read)
		.Times(1);

	deviceDriver.write(ADDR_LIST[0], 3);
}

TEST_F(DeviceDriverFixture, WriteAllDoneTest)
{
	EXPECT_CALL(mockFlash, read)
		.WillRepeatedly(Return(0x00));

	EXPECT_CALL(mockFlash, read)
		.Times(5);

	for (long address : ADDR_LIST) {
		deviceDriver.write(address, 3);
	}
}

TEST_F(DeviceDriverFixture, AppReadTest)
{
	app.setFlashMem(&mockFlash);
	EXPECT_CALL(mockFlash, read)
		.WillRepeatedly(Return(0x07));

	EXPECT_EQ("7 7 7 7 7", app.ReadAndPrint(ADDR_LIST[0], ADDR_LIST[4]));
}

TEST_F(DeviceDriverFixture, AppWriteTest)
{
	EXPECT_CALL(mockFlash, read)
		.Times(5);

	app.setFlashMem(&mockFlash);
	app.WriteAll(0x8);
}