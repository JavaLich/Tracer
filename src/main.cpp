#define CL_USE_DEPRECATED_OPENCL_2_0_APIS

#include <iostream>

#include <CL/cl.hpp>

#include "Application.h"

int main(int argc, char* argv[])
{
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	_ASSERT(platforms.size() > 0);

	auto platform = platforms.front();
	std::vector<cl::Device> devices;
	platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

	_ASSERT(devices.size() > 0);

	auto device = devices.front();
	auto vendor = device.getInfo<CL_DEVICE_VENDOR>();
	auto version = device.getInfo<CL_DEVICE_VERSION>();

	std::cout << vendor << " " << version << std::endl;

	Application* app = new Application();
	app->run();
	delete app;

	return 0;
}