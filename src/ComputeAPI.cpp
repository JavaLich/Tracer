#include "ComputeAPI.h"

#include <string>
#include <sstream>
#include <fstream>

ComputeAPI::ComputeAPI()
{
	init();
}

void ComputeAPI::render(uint32_t* pixels, int width, int height)
{
	cl_int err;
	cl::Buffer buffer1 = cl::Buffer(context, CL_MEM_WRITE_ONLY, (size_t)width * height * sizeof(uint32_t), pixels, &err);
	cl::Kernel kernel(program, "render", &err);
	kernel.setArg(0, buffer1);
	cl::CommandQueue queue(context, device);
	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange((size_t)width * height));
	queue.enqueueReadBuffer(buffer1, CL_TRUE, 0, (size_t)width * height * sizeof(uint32_t), pixels);
}

void ComputeAPI::init()
{
	initDevice();
	initProgram();
}

void ComputeAPI::initDevice()
{
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	if (platforms.size() == 0) {
		std::cerr << "No OpenCL platforms present!" << std::endl;
		return;
	}

	platform = platforms.front();
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
	context = cl::Context({ device });
}

void ComputeAPI::initProgram()
{
	std::ifstream f("kernels/tracer.cl");
	std::string code;
	if (f) {
		std::ostringstream ss;
		ss << f.rdbuf();
		code = ss.str();
	}
	cl::Program::Sources source;
	source.push_back({ code.c_str(), code.length() });
	program = cl::Program(context, source);
	if (program.build("-cl-std=CL1.2") != CL_SUCCESS) {
		std::cerr << "Error building " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
		return;
	}
	
}

