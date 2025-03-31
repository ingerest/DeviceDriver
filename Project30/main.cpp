#include "gmock/gmock.h"
#include "DeviceDriver.h"
#include "FlashMemoryDevice.h"
#include "Application.h"

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

TEST(ApplicationTest, ReadAndPrintReadsAndPrint) {
    MockFlashMemoryDevice mockHardware;
    DeviceDriver driver(&mockHardware);
    Application app(&driver);

    EXPECT_CALL(mockHardware, read(testing::_))
        .Times(15)
        .WillRepeatedly([](long addr) {
        return (unsigned char)(addr + 10);
            });

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    app.ReadAndPrint(0x00, 0x02);

    std::cout.rdbuf(old);

    std::string output = buffer.str();
    EXPECT_NE(output.find("Address: 0x0, Value: 10"), std::string::npos);
    EXPECT_NE(output.find("Address: 0x1, Value: 11"), std::string::npos);
    EXPECT_NE(output.find("Address: 0x2, Value: 12"), std::string::npos);
}

TEST(ApplicationTest, WriteAllWritesCorrectly) {
    MockFlashMemoryDevice mockHardware;
    DeviceDriver driver(&mockHardware);
    Application app(&driver);

    int value = 42;
    for (long addr = 0x00; addr <= 0x04; ++addr) {
        EXPECT_CALL(mockHardware, write(addr, (unsigned char)value))
            .Times(1);
        EXPECT_CALL(mockHardware, read(addr))
            .Times(1)
            .WillOnce(testing::Return((unsigned char)value));
    }

    app.WriteAll(value);
}

int main()
{
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}