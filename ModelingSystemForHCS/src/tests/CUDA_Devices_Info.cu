#ifndef CUDA_DEVICES_INFO_CU
#define CUDA_DEVICES_INFO_CU

#include <cuda.h>
#include <iostream>
#include <stdio.h>
#include <sstream> // std::ostringstream
#include "CUDA_Devices_Info.h"

extern "C"
void printGpuParameters(std::string prefixDescr) {	
	int deviceCount;
	cudaDeviceProp devProp;

	cudaGetDeviceCount(&deviceCount);
	
	std::ostringstream ss;
	ss << "------- Printing CUDA-compatible device properties -------" << std::endl;
    ss << prefixDescr << std::endl;
	ss << "Found " << deviceCount << " cuda compatible devices" << std::endl;

	for(int device = 0; device < deviceCount; device++){
		cudaGetDeviceProperties(&devProp, device);

		ss << "  --- Device " << device << " ---" << std::endl;
		ss << "Compute capability              : " << devProp.major << "." << devProp.minor << std::endl;
		ss << "Name                            : " << devProp.name << std::endl;
		ss << "Total Global Memory             : " << devProp.totalGlobalMem << " bytes ("<< devProp.totalGlobalMem  / 1024.0 / 1024.0 / 1024.0 << " Gb)" << std::endl;
		ss << "Shared memory per block         : " << devProp.sharedMemPerBlock << " bytes" << std::endl;
		ss << "Shared memory per multiprocessor: " << devProp.sharedMemPerMultiprocessor << " bytes" << std::endl;
		ss << "Registers per block             : " << devProp.regsPerBlock << std::endl;
		ss << "Registers per multiprocessor    : " << devProp.regsPerMultiprocessor << std::endl;
		ss << "Warp size                       : " << devProp.warpSize << std::endl;
		ss << "Max threads per block           : " << devProp.maxThreadsPerBlock << std::endl;
		ss << "Total constant memory           : " << devProp.totalConstMem << " bytes" << std::endl;
		ss << "Clock rate                      : " << devProp.clockRate << " kHz" << std::endl;
		ss << "Global memory bus width         : " << devProp.memoryBusWidth << " bits" << std::endl;
		ss << "Peak memory clock frequency     : " << devProp.memoryClockRate << " kHz" << std::endl;
		ss << "Texture alignment               : " << devProp.textureAlignment << std::endl;
		ss << "Device overlap                  : " << devProp.deviceOverlap << std::endl;
		ss << "Multiprocessor count            : " << devProp.multiProcessorCount << std::endl;
		ss << "Max threads dim                 : " << devProp.maxThreadsDim[0] << " " << devProp.maxThreadsDim[1] << " " << devProp.maxThreadsDim[2] << std::endl;
		ss << "Max threads per block           : " << devProp.maxThreadsPerBlock << std::endl;
		ss << "Max threads per multiprocessor  : " << devProp.maxThreadsPerMultiProcessor << std::endl;
		ss << "Max grid num                    : " << devProp.maxGridSize[0] << " " << devProp.maxGridSize[1] << " " << devProp.maxGridSize[2] << std::endl;
	}

    std::cout << ss.str();
}

#endif