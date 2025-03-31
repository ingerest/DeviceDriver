#pragma once
#include "FlashMemoryDevice.h"
#include <stdexcept>

class ReadFailException : public std::runtime_error {
public:
    explicit ReadFailException(const std::string& msg) : std::runtime_error(msg) {}
};

class WriteFailException : public std::runtime_error {
public:
    explicit WriteFailException(const std::string& msg) : std::runtime_error(msg) {}
};

class DeviceDriver
{
public:
    DeviceDriver(FlashMemoryDevice* hardware);
    int read(long address);
    void write(long address, int data);

protected:
    FlashMemoryDevice* m_hardware;
};