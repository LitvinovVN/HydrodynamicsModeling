#ifndef TEST_FUNCTIONS_H
#define TEST_FUNCTIONS_H

#include "../Grid3DSrc/Classes.h"

void runTests(LaunchSettings* settings);

void matmAlgsTesting();

void CUDA_Devices_Info();

void transferFragmentXOZTesting02();

void transferFragmentPlaneXOZTesting03();

void transferFragmentXOYTesting04();

void transferFragmentPlaneXOYTesting05();

void transferFragmentYOZTesting06();

void transferFragmentPlaneYOZTesting07();

void matmAlgsTestingMultithreading();

#endif