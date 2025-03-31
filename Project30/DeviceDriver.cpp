#include "DeviceDriver.h"
#include <thread>  
#include <chrono>

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{
}

int DeviceDriver::read(long address)
{
    const int numReads = 5;
    const int delayMs = 200;
    int values[numReads];

    for (int i = 0; i < numReads; ++i) {
        values[i] = (int)(m_hardware->read(address));
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }

    for (int i = 1; i < numReads; ++i) {
        if (values[i] != values[0]) {
            throw ReadFailException("Read values are inconsistent.");
        }
    }

    return values[0];
}

void DeviceDriver::write(long address, int data)
{
    // TODO: implement this method
    m_hardware->write(address, (unsigned char)data);

    int readBack = (int)(m_hardware->read(address));
    if (readBack != data) {
        throw WriteFailException("Data verification failed after write.");
    }
}