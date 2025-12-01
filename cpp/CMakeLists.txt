cmake_minimum_required(VERSION 3.20)
project(cpp)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 23) # 使用 C++23 标准
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# for all *.cpp, consider adding it as a executable
file(GLOB SOURCES "*.cpp")
# add -g flag for debugging
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
add_subdirectory(design_pattern)
foreach(SOURCE ${SOURCES})
    get_filename_component(EXECUTABLE ${SOURCE} NAME_WE)
    add_executable(${EXECUTABLE} ${SOURCE})
endforeach()
