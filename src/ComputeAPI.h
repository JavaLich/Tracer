#define CL_USE_DEPRECATED_OPENCL_2_0_APIS

#include <CL/cl.hpp>

#include <iostream>

typedef cl_float3 Vec3;

class ComputeAPI {

public:
	ComputeAPI();

	void render(uint32_t* pixels, int width, int height);

private:
	void init();
	void initDevice();
	void initProgram();

private:
	cl::Device device;
	cl::Platform platform;
	cl::Context context;
	cl::Program program;
};