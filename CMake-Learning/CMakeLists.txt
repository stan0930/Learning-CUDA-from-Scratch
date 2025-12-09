cmake_minimum_required(VERSION 3.10)

# 项目名称和版本
# Project name and version
project(CMakeLearning VERSION 1.0)

# 设置 C++ 标准
# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加子目录 (MathLib)
# Add subdirectory (MathLib)
add_subdirectory(MathLib)

# 创建可执行文件
# Create executable
add_executable(CMakeLearningApp main.cpp)

# 链接 MathLib 库到可执行文件
# Link MathLib to the executable
target_link_libraries(CMakeLearningApp PUBLIC MathLib)

# 包含 MathLib 的头文件路径 (可选，如果头文件引用方式不同)
# Include directory for MathLib headers
target_include_directories(CMakeLearningApp PUBLIC "${PROJECT_SOURCE_DIR}")
