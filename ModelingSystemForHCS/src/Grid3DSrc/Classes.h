#ifndef CLASSES
#define CLASSES

#include <mpi.h>

#include <iostream>
#include <functional> // std::ref
#include <map>
#include <vector>
#include <chrono>
#include <algorithm>    // std::sort


#include "Enums.cpp"
#include "Timer.cpp"
#include "SimpleStatistics.cpp"

#pragma region LinearArrays
#include "LinearArray1D.cpp"
#include "LinearArray2D.cpp"
#include "LinearArray3D.cpp"
#include "LinearArrays2D.cpp"
#include "LinearArrays3D.cpp"
#pragma endregion LinearArrays

#pragma region Devices
#include "ComputingDevice.cpp"
#include "ComputingDeviceCPU.cpp"
#include "ComputingDeviceGPU.cpp"
#pragma endregion Devices

#pragma region Cluster
#include "ComputingNode.cpp"
#include "ComputingCluster.cpp"
#pragma endregion Cluster

#pragma region Grid
#include "Grid2DFragmentXY.cpp"
#include "Grid2DFragmentYZ.cpp"
#include "Grid2DFragmentXZ.cpp"

#include "Grid2DTransferPlaneXZ.cpp"
#include "Grid3DFragment.cpp"
#include "GridBlock3DByDevice.cpp"
#include "GridBlock3DByNode.cpp"
#include "Grid3D.cpp"
#include "GridHelper.cpp"
#pragma endregion Grid

#include "Utils.h"
#include "LaunchSettings.cpp"

#endif