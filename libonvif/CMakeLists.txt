#*******************************************************************************
# libonvif/libonvif/CMakeLists.txt
#
# Copyright (c) 2023, 2024 Stephen Rhodes 
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#
#******************************************************************************/

cmake_minimum_required(VERSION 3.17)

project(libonvif VERSION 3.2.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (DEFINED ENV{LIBXML2_INCLUDE_DIRS})
    set(LIBXML2_INCLUDE_DIRS $ENV{LIBXML2_INCLUDE_DIRS})
    set(LIBXML2_LIBRARIES $ENV{LIBXML2_LIBRARIES})
else()
    find_package(LibXml2 REQUIRED)
endif()

add_subdirectory(pybind11)

pybind11_add_module(pyonvif
    src/onvif.cpp
    src/onvif.c
    src/cencode.c
    src/sha1.c
)

set_target_properties(pyonvif PROPERTIES
    OUTPUT_NAME libonvif
)

target_link_libraries(pyonvif PRIVATE
    ${LIBXML2_LIBRARIES}
)

message("-- LIBXML2_INCLUDE_DIRS: " ${LIBXML2_INCLUDE_DIRS})
message("-- LIBXML2_LIBRARIES: " ${LIBXML2_LIBRARIES})

target_include_directories(pyonvif PUBLIC
    include
    ${LIBXML2_INCLUDE_DIRS}
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set_target_properties(pyonvif PROPERTIES
        BUILD_RPATH "$ORIGIN"
        BUILD_RPATH_USE_ORIGIN TRUE
        INSTALL_RPATH "$ORIGIN"
        INSTALL_RPATH_USE_ORIGIN TRUE
    )
endif()

# --- 新增：生成 C++ 动态库 (DLL) ---

# 定义源文件列表（排除掉专门给 python 用的 onvif.cpp，通常 C 库核心在 .c 文件中）
set(LIB_SOURCES
    src/onvif.c
    src/cencode.c
    src/sha1.c
)

# 添加动态库目标
add_library(onvif_cpp SHARED ${LIB_SOURCES})

# 设置输出名称为 libonvif (生成的将是 libonvif.dll)
set_target_properties(onvif_cpp PROPERTIES 
    OUTPUT_NAME "onvif"
    PREFIX "lib"
)

# 链接 libxml2
target_link_libraries(onvif_cpp PRIVATE 
    ${LIBXML2_LIBRARIES}
)

# 指定头文件目录
target_include_directories(onvif_cpp PUBLIC 
    include
    ${LIBXML2_INCLUDE_DIRS}
)

# Windows 下导出符号处理（非常重要）
if(WIN32)
    target_compile_definitions(onvif_cpp PRIVATE "-D_CRT_SECURE_NO_WARNINGS")
    # 如果源码中没有 __declspec(dllexport)，建议加上这个让编译器导出所有函数
    set_target_properties(onvif_cpp PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# 1. 安装 Python 模块 (.pyd)
install(TARGETS pyonvif
    DESTINATION .
)

# 2. 安装 C++ 动态库 (.dll 和 .lib)
install(TARGETS onvif_cpp
    RUNTIME DESTINATION bin    # .dll 放在 bin
    LIBRARY DESTINATION lib    # .so 放在 lib
    ARCHIVE DESTINATION lib    # .lib 放在 lib
)

# 3. 安装头文件 (include 目录下的所有内容)
install(DIRECTORY include/ 
    DESTINATION include
)