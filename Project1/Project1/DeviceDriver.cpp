#include "DeviceDriver.h"
#include <thread>

using namespace std;

const int REPAT_READ_NUM = 5;

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{}

int DeviceDriver::read(const long& address)
{
    int firstRead = (int)(m_hardware->read(address));
    for (int i = 1; i < REPAT_READ_NUM; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(200));
        if (firstRead != (int)(m_hardware->read(address)))
        {
            throw ReadFailException();
        }
    }
    return firstRead;
}

void DeviceDriver::write(const long& address, const int& data)
{
    // TODO: implement this method
    m_hardware->write(address, (unsigned char)data);
}