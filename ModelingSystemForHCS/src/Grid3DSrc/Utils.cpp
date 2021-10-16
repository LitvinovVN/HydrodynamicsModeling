#ifndef UTILS
#define UTILS

#include "Utils.h"

#include <iostream> 
#include <thread>

#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <iomanip> // put_time


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
    auto now = std::chrono::system_clock::now();
    time_t tt = std::chrono::system_clock::to_time_t(now);

    std::cout << std::put_time(std::localtime(&tt), "%X");
}

void printTime(std::string timePointDescr){
    auto now = std::chrono::system_clock::now();
    time_t tt = std::chrono::system_clock::to_time_t(now);  

    std::cout << std::put_time(std::localtime(&tt), "%X") << ": " << timePointDescr << std::endl;    
}

#endif