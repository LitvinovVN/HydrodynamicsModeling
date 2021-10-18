#ifndef UTILS
#define UTILS

#include "Utils.h"

#include <iostream> 
#include <thread>
#include <mpi.h>
#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <iomanip> // put_time

#ifdef _WIN32
    #include <Windows.h>
    #include <tchar.h>
#elif _WIN64
    #include <Windows.h>
    #include <tchar.h>
#elif WIN32
    #include <Windows.h>
    #include <tchar.h>
#elif WIN64
    #include <Windows.h>
    #include <tchar.h>
#else
    // code for linux
    #include <unistd.h>// getpid
    #include <cstring>// memset
#endif


void app_init(int argc, char* argv[]){
    auto pid = getpid();
    int nHardwareThreads = std::thread::hardware_concurrency();
    fprintf(stderr, "Process ID: %d. Hardware threads: %d \n", pid, nHardwareThreads);

    printf("argc: %d\n", argc);
    printf("&argc: %p\n", (void*)&argc);
        
    for(int i=0;i<argc;i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }    
}

/// <summary>
/// Возвращает имя хоста
/// </summary>
void GetMachineName(char machineName[150])
{
    char Name[150];
    int i = 0;

#ifdef WIN32
    TCHAR infoBuf[150];
    DWORD bufCharCount = 150;
    memset(Name, 0, 150);
    if (GetComputerName(infoBuf, &bufCharCount))
    {
        for (i = 0; i < 150; i++)
        {
            Name[i] = infoBuf[i];
        }
    }
    else
    {
        strcpy(Name, "Unknown_Host_Name");
    }
#else
    memset(Name, 0, 150);
    gethostname(Name, 150);
#endif
    strncpy(machineName, Name, 150);
}

void mpi_init(int argc, char* argv[], int mpi_thread_type, int& provided, int& rank, int& size) {
    MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    char hostname[50];
    GetMachineName(hostname);
    auto pid = getpid();
    int nHardwareThreads = std::thread::hardware_concurrency();
    fprintf(stderr, "Hostname: %s. MPI rank: %d. Process ID: %d. Hardware threads: %d \n", hostname, rank, pid, nHardwareThreads);

    if (rank == 0) {
        printf("argc: %d\n", argc);
        printf("&argc: %p\n", (void*)&argc);

        for (int i = 0; i < argc; i++)
        {
            printf("argv[%d]: %s\n", i, argv[i]);
        }

        printf("mpi_thread_type: %d\n", mpi_thread_type);

        printf("provided: %d\n", provided);
        printf("&provided: %p\n", &provided);

        printf("size: %d\n", size);
        printf("&size: %p\n", &size);

        if (provided < MPI_THREAD_FUNNELED)
        {
            printf("The threading support level is lesser than that demanded.\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        else
        {
            printf("The threading support level: MPI_THREAD_FUNNELED\n");
        }
    }
}


void printTime() {
    // Закомментировано, т.к. std::put_time не реализован в gcc 4.x
    /*auto now = std::chrono::system_clock::now();
    time_t tt = std::chrono::system_clock::to_time_t(now);

    std::cout << std::put_time(std::localtime(&tt), "%X");*/
    //std::cout << std::localtime(&tt);

    time_t t = time(0);
    char buffer[9] = { 0 };

    strftime(buffer, 9, "%H:%M:%S", localtime(&t));
    std::cout << std::string(buffer);
}

#endif