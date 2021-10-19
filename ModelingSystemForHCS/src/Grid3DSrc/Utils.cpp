#ifndef UTILS_CPP
#define UTILS_CPP

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


LaunchSettings* app_init(int argc, char* argv[]){
    setlocale(LC_CTYPE, "rus"); // вызов функции настройки локали
    std::cout << "--- Modeling System for HCS";
    std::cout << " (launched at "; printTime(); std::cout << ") ---";
    std::cout << std::endl;
        
    auto settings = new LaunchSettings{ argc, argv };

    

    return settings;
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