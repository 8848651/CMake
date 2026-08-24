# 顶层 CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(Socket_Server)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

# 将子目录添加到项目中（库）
add_subdirectory(stl)
add_subdirectory(muduo)

# 主程序：正式入口（这里只有一个 main 可执行目标）
add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE muduo stl_core)

# 测试开关：cmake -DBUILD_TESTING=OFF 可关闭测试构建
option(BUILD_TESTING "Build the test suite" ON)
if(BUILD_TESTING)
    enable_testing()        # 启用 ctest 支持
    add_subdirectory(test)  # test 里定义独立的测试可执行文件（有自己 main）
endif()
