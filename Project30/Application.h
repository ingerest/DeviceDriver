#pragma once
#include "DeviceDriver.h"

class Application
{
public:
    Application(DeviceDriver* driver);

    void ReadAndPrint(long startAddr, long endAddr);
    void WriteAll(int value);

private:
    DeviceDriver* m_driver;
};