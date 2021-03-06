#include "ComputeAPI.h"

#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>

ComputeAPI::ComputeAPI()
{
	init();
}

void ComputeAPI::render(uint32_t* pixels, unsigned int width, unsigned int height, Scene* scene, cl_float3* rot)
{
	cl_int err;
	cl::Buffer pixelBuffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, (size_t)width * height * sizeof(uint32_t), nullptr, &err);
	cl::Buffer sceneBuffer = cl::Buffer(context, CL_MEM_READ_ONLY, (size_t)sizeof(Scene), nullptr, &err);
	cl::Buffer rotBuffer = cl::Buffer(context, CL_MEM_READ_ONLY, (size_t)3 * sizeof(cl_float3), nullptr, &err);
	cl::CommandQueue queue(context, device);
	queue.enqueueWriteBuffer(sceneBuffer, CL_FALSE, 0, (size_t)sizeof(Scene), (void*)scene);
	queue.enqueueWriteBuffer(rotBuffer, CL_FALSE, 0, (size_t)3 * sizeof(cl_float3), rot);
	cl::Kernel kernel(program, "render", &err);
	kernel.setArg(0, pixelBuffer);
	kernel.setArg(1, width);
	kernel.setArg(2, height);
	kernel.setArg(3, rotBuffer);
	kernel.setArg(4, sceneBuffer);
	
	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange((size_t)width * height));
	//queue.enqueueTask(kernel);
	queue.enqueueReadBuffer(pixelBuffer, CL_TRUE, 0, (size_t)width * height * sizeof(uint32_t), pixels);
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
	else {
		std::cout << "Couldn't find kernel file" << std::endl;
	}
	cl::Program::Sources source;
	source.push_back({ code.c_str(), code.length() });
	program = cl::Program(context, source);
	if (program.build("-cl-std=CL1.2") != CL_SUCCESS) {
		std::cerr << "Error building " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
		return;
	}
	
}
