#ifndef CUDA_DEVICES_INFO_CPP
#define CUDA_DEVICES_INFO_CPP

#include "testFunctions.h"
#include "CUDA_Devices_Info.h"

void CUDA_Devices_Info()
{
	std::cout << "------ CUDA_Devices_Info() -------" << std::endl;
	int t = 5;
	printGpuParameters("");
#ifdef CUDA
	printGpuParameters("");
#endif // CUDA		
}

#endif