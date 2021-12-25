# CMakeList.txt: проект CMake для mrsu; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project ("mrsu")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (mrsu "mrsu.cpp" )

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
