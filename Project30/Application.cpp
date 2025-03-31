#include "Application.h"
#include <iostream>

Application::Application(DeviceDriver* driver)
    : m_driver(driver) {
}

void Application::ReadAndPrint(long startAddr, long endAddr)
{
    for (long addr = startAddr; addr <= endAddr; ++addr) {
        try {
            int value = m_driver->read(addr);
            std::cout << "Address: 0x" << std::hex << addr << ", Value: " << std::dec << value << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "Read failed at address 0x" << std::hex << addr << ": " << e.what() << std::endl;
        }
    }
}

void Application::WriteAll(int value)
{
    for (long addr = 0x00; addr <= 0x04; ++addr) {
        m_driver->write(addr, value);
    }
}