#define CL_USE_DEPRECATED_OPENCL_2_0_APIS

#include <CL/cl.hpp>

#include <iostream>


__declspec(align(64)) struct Sphere {
	cl_float3 position;
	cl_uint3 color = cl_uint3{ 255, 255, 255 };
	cl_float radius = 1.0f;
};

struct Scene {
	cl_float3 cameraPos = cl_float3{ 0.0f, 0.0f, 0.0f };
	cl_float screenDistance = 1.0f;
};

struct Light {
	cl_float3 color = cl_float3{ 0.5f, 0.5f, 0.5f };
	cl_float3 position = cl_float3{ 0.0f, 0.0f, 0.0f };
};

class ComputeAPI {

public:
	ComputeAPI();

	void render(uint32_t* pixels, Sphere* spheres, unsigned int sphereCount, unsigned int width, unsigned int height, Scene scene, Light* lights, unsigned int light_count, cl_float3* rot);

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