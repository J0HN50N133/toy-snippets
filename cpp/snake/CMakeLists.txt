cmake_minimum_required(VERSION 3.15)
project(snake)

include(FetchContent)

# raylib
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

set(CMAKE_CXX_STANDARD 23)
include_directories(./)
add_executable(snake snake.cpp)
target_link_libraries(snake PRIVATE raylib)
target_include_directories(snake PRIVATE ${raylib_SOURCE_DIR}/src)

