#ifndef UTILS_H
#define UTILS_H

#include <iostream>

void app_init(int argc, char* argv[]);

void GetMachineName(char machineName[150]);
void mpi_init(int argc, char* argv[], int mpi_thread_type, int& provided, int& rank, int& size);
void printTime();

#endif