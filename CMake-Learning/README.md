# CMake 入门实战项目

这是一个简单的 C++ 项目，旨在帮助你学习 CMake 的基本用法。项目包含一个主程序 (`CMakeLearningApp`) 和一个静态库 (`MathLib`)。

## 📂 项目结构

```
CMake-Learning/
├── CMakeLists.txt          (根目录 CMake 配置文件)
├── main.cpp                (主程序入口，调用 MathLib)
├── MathLib/                (静态库目录)
│   ├── CMakeLists.txt      (库的 CMake 配置文件)
│   ├── MathFunctions.h     (头文件)
│   └── MathFunctions.cpp   (实现文件)
└── README.md               (本说明文件)
```

## 🚀 关键 CMake 指令解析

### 1. 根目录 `CMakeLists.txt`

```cmake
# 规定 CMake 最低版本
cmake_minimum_required(VERSION 3.10)

# 定义项目名称和版本
project(CMakeLearning VERSION 1.0)

# 添加子目录
# 这行命令告诉 CMake 去 MathLib 目录下找另一个 CMakeLists.txt
add_subdirectory(MathLib)

# 定义可执行文件
# 告诉 CMake 用 main.cpp 生成一个叫 CMakeLearningApp 的程序
add_executable(CMakeLearningApp main.cpp)

# 链接库
# 将 MathLib 库链接到我们的程序上，这样 main.cpp 才能调用 MathLib 里的函数
target_link_libraries(CMakeLearningApp PUBLIC MathLib)
```

### 2. 子目录 `MathLib/CMakeLists.txt`

```cmake
# 定义一个静态库
# 告诉 CMake 用 MathFunctions.cpp 生成一个叫 MathLib 的静态库 (.lib / .a)
add_library(MathLib STATIC MathFunctions.cpp)
```

## 🔨 如何构建和运行

你可以使用命令行来构建这个项目。

1.  **创建构建目录** (保持源码目录整洁):
    ```bash
    mkdir build
    cd build
    ```

2.  **生成构建系统** (运行 CMake):
    ```bash
    cmake ..
    ```

3.  **编译项目**:
    ```bash
    cmake --build .
    ```

4.  **运行程序**:
    *   **Windows**:
        ```cmd
        Debug\CMakeLearningApp.exe
        ```
    *   **Linux/Mac**:
        ```bash
        ./CMakeLearningApp
        ```

## ✨ 这个项目教会了你什么？

1.  如何使用 `add_executable` 创建程序。
2.  如何使用 `add_library` 创建库。
3.  如何使用 `add_subdirectory` 组织大型项目。
4.  如何使用 `target_link_libraries` 将它们连接在一起。
