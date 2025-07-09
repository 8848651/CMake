# 收集cpp文件
file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# 将上述收集的文件做一个库
add_library(v1 STATIC ${SRC_FILES})

# 规定上面cpp库链接时去哪找类的定义，规定链接时头文件去哪
target_include_directories(v1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 将别的库加入现库中
target_link_libraries(v1 PRIVATE stl_core)
