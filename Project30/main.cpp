#include "gmock/gmock.h"
#include "DeviceDriver.h"
#include "FlashMemoryDevice.h"

class MockFlashMemoryDevice : public FlashMemoryDevice
{
public:
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

TEST(DeviceDriverTest, ReadReturnsSameValue)
{
	MockFlashMemoryDevice mock;
	DeviceDriver driver(&mock);

	long address = 0x00;
	unsigned char expectedValue = 42;

	EXPECT_CALL(mock, read(address))
		.Times(5)
		.WillRepeatedly(testing::Return(expectedValue));

	int result = driver.read(address);

	EXPECT_EQ(result, expectedValue);
}

TEST(DeviceDriverTest, ReadThrowsExceptionIfValues) {
    MockFlashMemoryDevice mock;
    DeviceDriver driver(&mock);

    constexpr long address = 0x00;

    testing::Sequence seq;
    EXPECT_CALL(mock, read(address))
        .InSequence(seq)
        .WillOnce(testing::Return(42))
        .WillOnce(testing::Return(42))
        .WillOnce(testing::Return(99))
        .WillOnce(testing::Return(42))
        .WillOnce(testing::Return(42));

    EXPECT_THROW({
        driver.read(address);
        }, ReadFailException);
}

TEST(DeviceDriverTest, WriteSuccess) {
    MockFlashMemoryDevice mock;
    DeviceDriver driver(&mock);

    long address = 0x10;
    int data = 77;

    EXPECT_CALL(mock, write(address, (unsigned char)data))
        .Times(1);

    EXPECT_CALL(mock, read(address))
        .Times(1)
        .WillOnce(testing::Return((unsigned char)data));

    driver.write(address, data);
}

TEST(DeviceDriverTest, WriteThrowsException) {
    MockFlashMemoryDevice mock;
    DeviceDriver driver(&mock);

    long address = 0x10;
    int data = 77;

    EXPECT_CALL(mock, write(address, (unsigned char)data))
        .Times(1);

    EXPECT_CALL(mock, read(address))
        .Times(1)
        .WillOnce(testing::Return((unsigned char) data + 1));

    EXPECT_THROW({
        driver.write(address, data);
        }, WriteFailException);
}

int main()
{
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}