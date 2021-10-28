#ifndef CUDA_DEVICES_INFO_CPP
#define CUDA_DEVICES_INFO_CPP

#include "testFunctions.h"
#include "CUDA_Devices_Info.h"

void CUDA_Devices_Info()
{
	int t = 5;
#ifdef CUDA
	printGpuParameters("");
#endif // CUDA		
}

#endif