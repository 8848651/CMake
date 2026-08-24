set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 独立的测试可执行文件：有自己的 main（src/test_main.cpp），
# 与顶层 src/main.cpp 互不冲突（CMake 允许多个可执行目标）
add_executable(test_main src/test_main.cpp)

# 链接被测试的库：muduo + stl_core（纯头文件 STL 库）
target_link_libraries(test_main PRIVATE muduo stl_core)

# 若测试自身需要公共头文件（如自定义断言/辅助函数），在这里暴露
target_include_directories(test_main PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 注册到 ctest：之后可用 `ctest` 或 VS Code 测试视图一键运行
add_test(NAME test_main COMMAND test_main)