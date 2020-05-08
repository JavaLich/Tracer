#define CL_USE_DEPRECATED_OPENCL_2_0_APIS

#include <CL/cl.hpp>

#include <iostream>


struct Sphere {
	cl_float3 position;
	cl_uint3 color;
	cl_float radius;
};

class ComputeAPI {

public:
	ComputeAPI();

	void render(uint32_t* pixels, Sphere* spheres, unsigned int sphereCount, unsigned int width, unsigned int height);

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