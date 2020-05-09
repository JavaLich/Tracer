#define CL_USE_DEPRECATED_OPENCL_2_0_APIS

#include <CL/cl.hpp>

#include <iostream>


struct Sphere {
	cl_float3 position;
	cl_uint3 color;
	cl_float radius;
};

struct Scene {
	cl_float3 cameraPos = cl_float3{ 0.0f, 0.0f, 0.0f };
	cl_float3 viewDir = cl_float3{ 0.0f, 0.0f, 1.0f };
	cl_float screenDistance = 1.0f;
};

class ComputeAPI {

public:
	ComputeAPI();

	void render(uint32_t* pixels, Sphere* spheres, unsigned int sphereCount, unsigned int width, unsigned int height, Scene scene);

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