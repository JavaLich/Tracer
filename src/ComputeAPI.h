#define CL_USE_DEPRECATED_OPENCL_2_0_APIS

#include <CL/cl.hpp>

#include <iostream>

class ComputeAPI {

public:
	ComputeAPI();

private:
	void init();
	void initDevice();

private:
	cl::Device device;

};