set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 收集cpp文件（注意：file(GLOB)不推荐用于正式项目，建议手动列出文件）
file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# 将上述收集的文件做一个静态库
add_library(muduo2 STATIC ${SRC_FILES})

# ========== 关键修改：PRIVATE 改为 PUBLIC ==========
# 原因：PUBLIC 表示 muduo 自己用这个头文件路径，且依赖 muduo 的目标（如main）也能使用
target_include_directories(muduo2 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include  # 对外暴露include目录
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src      # 仅v5内部用的src目录（可选）
)

# 将别的库加入现库中（stl_core如果有头文件，建议也设为PUBLIC）
#target_link_libraries(muduo2 PRIVATE stl_core)

# 演示：echo server（用 muduo2 的 CRTP 接口）
add_executable(muduo2_demo demo/echo_server.cpp)
target_link_libraries(muduo2_demo PRIVATE muduo2)
