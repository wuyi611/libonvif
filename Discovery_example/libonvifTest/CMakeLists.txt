cmake_minimum_required(VERSION 3.16)
project(libonvifTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(libonvifTest main.cpp)

# --- 1. 手动设置 libxml2 路径 (根据你的 vcpkg 实际路径修改) ---
# 假设你的 vcpkg 在 G:/E/vcpkg
set(VCPKG_INSTALLED_PATH "D:/vcpkg/installed/x64-windows")

set(LIBXML2_INCLUDE_DIR "${VCPKG_INSTALLED_PATH}/include/libxml2")
set(LIBXML2_LIB         "${VCPKG_INSTALLED_PATH}/lib/libxml2.lib")
set(LIBXML2_DLL         "${VCPKG_INSTALLED_PATH}/bin/libxml2.dll")

# --- 2. 设置 libonvif 路径 ---
set(ONVIF_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libonvif/include")
set(ONVIF_LIB_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/libonvif/lib")
# 假设你编出的 dll 叫 libonvif.dll
set(ONVIF_DLL         "${CMAKE_CURRENT_SOURCE_DIR}/libonvif/bin/libonvif.dll")

# --- 3. 添加头文件包含路径 ---
target_include_directories(${PROJECT_NAME} PRIVATE
    ${ONVIF_INCLUDE_DIR}
    ${LIBXML2_INCLUDE_DIR}
)

# --- 4. 链接库文件 ---
# 直接链接 .lib 文件的绝对路径是最稳妥的
target_link_libraries(${PROJECT_NAME} PRIVATE
    "${ONVIF_LIB_DIR}/onvif.lib"
    "${LIBXML2_LIB}"
)

# --- 5. 自动拷贝 DLL 到生成目录 (Windows 运行必备) ---
# 这样你点击 Qt 的“运行”才不会报找不到 dll
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${LIBXML2_DLL}" $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${ONVIF_DLL}"   $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# --- 6. 安装配置 ---
include(GNUInstallDirs)
install(TARGETS libonvifTest
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
