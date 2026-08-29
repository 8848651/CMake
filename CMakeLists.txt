# 顶层 CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(Socket_Server)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

# 将子目录添加到项目中（库）：muduo 与 muduo2 是两套独立实现，都保留可用
add_subdirectory(stl)      # 纯头文件 STL 库（stl_core），被 test_main 使用
add_subdirectory(muduo)    # 旧版 tcpserver（非模板，全局命名空间）
add_subdirectory(muduo2)   # 重写版 tcpserver（模板，namespace muduo）

# 主程序：正式入口（这里只有一个 main 可执行目标）
# main.cpp 使用 muduo 的头文件（../muduo/include/version.h），故链接 muduo
add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE muduo)

# 测试开关：cmake -DBUILD_TESTING=OFF 可关闭测试构建
option(BUILD_TESTING "Build the test suite" ON)
if(BUILD_TESTING)
    enable_testing()        # 启用 ctest 支持
    add_subdirectory(test)  # test 里定义独立的测试可执行文件（有自己 main）
endif()
