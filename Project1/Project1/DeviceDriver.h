#pragma once
#include "FlashMemoryDevice.h"

class DeviceDriver
{
public:
    DeviceDriver(FlashMemoryDevice* hardware);
    int read(const long& address);
    void write(const long& address,const int& data);

protected:
    FlashMemoryDevice* m_hardware;
};