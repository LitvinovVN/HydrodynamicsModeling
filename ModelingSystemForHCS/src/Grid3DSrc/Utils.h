#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include "LaunchSettings.cpp"

LaunchSettings* app_init(int argc, char* argv[]);

void GetMachineName(char machineName[150]);

void printTime();

#endif