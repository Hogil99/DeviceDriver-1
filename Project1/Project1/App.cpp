#include <string>

#include "DeviceDriver.h"

class Application
{
public:
	std::string ReadAndPrint(const long& startAddr, const long& endAddr);
	void WriteAll(const int& value);
	void setFlashMem(FlashMemoryDevice* flashMem);
private:
	DeviceDriver *deviceDriver;
};

void Application::setFlashMem(FlashMemoryDevice* flashMem)
{
	deviceDriver = new DeviceDriver(flashMem);
}


std::string Application::ReadAndPrint(const long& startAddr, const long& endAddr) {
	std::string result = "";
	for (long i = startAddr; i <= endAddr; i++)
	{
		result += std::to_string(deviceDriver->read(i));
		result += " ";
	}
	result.pop_back();
	return result;
}

void Application::WriteAll(const int& value)
{
	for(long address : ADDR_LIST)
	{
		deviceDriver->write(address, value);
	}
}

