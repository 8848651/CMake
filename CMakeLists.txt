# 顶层 CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(Socket_Server)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# CMake刷新  CMake: Delete Cache and Reconfigure
add_library(common_main OBJECT src/main.cpp)

#将子目录添加到项目中
add_subdirectory(stl)
add_subdirectory(muduo/version_5)

add_executable(main $<TARGET_OBJECTS:common_main>)

#将库链接
target_link_libraries(main PRIVATE v5)
