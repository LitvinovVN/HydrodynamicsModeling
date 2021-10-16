#ifndef UTILS
#define UTILS

#include "Utils.h"

#include <iostream> 
#include <thread>

#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <iomanip> // put_time

#ifdef _WIN32
#elif _WIN64
#elif WIN32
#elif WIN64
#else
// code for linux
#include <unistd.h>// getpid
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