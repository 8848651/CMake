
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 创建 INTERFACE 库目标（无需源文件）
add_library(stl_core INTERFACE)

# 暴露头文件路径（使用 INTERFACE 传递依赖）
target_include_directories(stl_core SYSTEM INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)