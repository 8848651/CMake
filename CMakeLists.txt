# 安装最小版本
cmake_minimum_required(VERSION 3.16.3)

# 项目名称
project(Socket_Sever)

# 关闭编译警告
add_compile_options(-Wno-return-type)

# 指定C++版本
set(CMAKE_CXX_STANDARD 14)

# 指定头文件目录 PROJECT_SOURCE_DIR即CMakeLists所在目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 可执行文件生成的目录 放exe文件
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)

find_package(Threads REQUIRED)

add_executable(
    main
    ${SRC_LIST}
)
target_link_libraries(main PRIVATE Threads::Threads)