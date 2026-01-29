cmake_minimum_required(VERSION 3.16)

project(libonvifTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(libonvifTest main.cpp)

include(GNUInstallDirs)
install(TARGETS libonvifTest
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set(ONVIF_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libonvif/include")
set(ONVIF_LIB_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/libonvif/lib")
set(ONVIF_DLL         "${CMAKE_CURRENT_SOURCE_DIR}/libonvif/bin/libonvif.dll")


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${ONVIF_DLL}"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${ONVIF_INCLUDE_DIR}
)


target_link_libraries(${PROJECT_NAME} PRIVATE
    "${ONVIF_LIB_DIR}/onvif.lib"
)


