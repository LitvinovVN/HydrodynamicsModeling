cmake_minimum_required(VERSION 2.8)

project(ModelingSystemForHCS)

set(CMAKE_CXX_FLAGS "-std=c++11")

option(ENABLE_MPI "Enable MPI parallelization" ON)

if(ENABLE_MPI)
    find_package(MPI REQUIRED)
    include_directories(${MPI_INCLUDE_PATH})
    set(CMAKE_C_FLAGS "${CMAKE_FLAGS} ${MPI_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MPI_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MPI_EXE_LINKER_FLAGS}")
endif(ENABLE_MPI)


file(GLOB source_files
            "*.h"
            "*.cpp"
            "tests/*.cpp"            
            "tests/*.h"
            "tests/*/*.cpp"
            "tests/*/*.h"
            "Grid3DSrc/Utils.cpp"
    )

add_executable(myApp   ${source_files} )

if(ENABLE_MPI)
    target_link_libraries(myApp ${MPI_LIBRARIES})
endif(ENABLE_MPI)