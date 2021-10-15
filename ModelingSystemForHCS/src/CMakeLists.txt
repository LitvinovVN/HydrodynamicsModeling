project(ModelingSystemForHCS)

set(CMAKE_CXX_FLAGS "-std=c++11")

file(GLOB source_files
            "*.h"
            "*.cpp"
            "tests/*.cpp"
            "tests/*.h"
            "tests/*/*.cpp"
            "tests/*/*.h"
    )

add_executable(myApp   ${source_files} )

