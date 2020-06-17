#define CL_USE_DEPRECATED_OPENCL_2_0_APIS

#include <CL/cl.hpp>

#include <iostream>

#define SPHERE_COUNT 8

__declspec(align(64)) struct Sphere {
	cl_float3 position;
	cl_uint3 color = cl_uint3{ 255, 255, 255 };
	cl_float radius = 1.0f;
};

__declspec(align(1024)) struct Scene {
	Sphere spheres[SPHERE_COUNT];

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

	void render(uint32_t* pixels, unsigned int width, unsigned int height, Scene* scene, Light* lights, unsigned int light_count, cl_float3* rot);

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