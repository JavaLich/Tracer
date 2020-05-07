#include "ComputeAPI.h"

#include <string>
#include <sstream>

ComputeAPI::ComputeAPI()
{
	init();
}

void ComputeAPI::init()
{
	initDevice();
}

void ComputeAPI::initDevice()
{
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	if (platforms.size() == 0) {
		std::cerr << "No OpenCL platforms present!" << std::endl;
		return;
	}

	auto platform = platforms.front();
	std::vector<cl::Device> devices;
	platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

	if (devices.size() == 0) {
		std::cerr << "No OpenCL devices present!" << std::endl;
		return;
	}

	int count = 1;
	for (auto device : devices) {
		auto vendor = device.getInfo<CL_DEVICE_VENDOR>();
		auto version = device.getInfo<CL_DEVICE_VERSION>();
		auto name = device.getInfo<CL_DEVICE_NAME>();

		std::cout << count << ": " << vendor << name << version << std::endl;

		count++;
	}

	std::cout << "Choose a device: ";
	std::string line;
	int selection = 0;
	while (std::getline(std::cin, line))
	{
		std::stringstream ss(line);
		if (ss >> selection)
		{
			selection--;
			if (ss.eof() && selection >= 0 && selection < devices.size())
			{   // Success
				break;
			}
		}
		std::cout << "Error! Choose a device: ";
	}
	std::cout << "You chose: " << devices[selection].getInfo<CL_DEVICE_VENDOR>() << devices[selection].getInfo<CL_DEVICE_NAME>() << devices[selection].getInfo<CL_DEVICE_VERSION>() << std::endl;
	this->device = devices[selection];
}


