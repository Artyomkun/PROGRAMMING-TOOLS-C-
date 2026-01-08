cmake_minimum_required(VERSION 3.10)
project(ExpertGeometry3D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

add_definitions(-DUNICODE -D_UNICODE)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/include/directx")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/include/wsl")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/include/dxc")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/include/Qt5")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/include/Qt5/QtCore")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/include/Qt5/QtGui")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/include/Qt5/QtWidgets")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/lib")
find_path(WINDOWS_SDK_DIR NAMES um/d3d12.h PATHS "$ENV{WindowsSdkDir}" NO_DEFAULT_PATH)
message(STATUS "Searching for Windows SDK at: $ENV{WindowsSdkDir}")
if(WINDOWS_SDK_DIR)
    message(STATUS "Found Windows SDK at: ${WINDOWS_SDK_DIR}")
else()
    message(STATUS "Windows SDK not found via environment, trying default paths")
endif()

if(WINDOWS_SDK_DIR)
    message(STATUS "Found Windows SDK at: ${WINDOWS_SDK_DIR}")
    include_directories("${WINDOWS_SDK_DIR}Include/$ENV{WindowsSDKVersion}um")
    include_directories("${WINDOWS_SDK_DIR}Include/$ENV{WindowsSDKVersion}shared")
else()
    include_directories("C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/um")
    include_directories("C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/shared")
    link_directories("C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64")
endif()

find_program(DXC_EXECUTABLE dxc PATHS
    "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/tools/directx-dxc"
    "D:/Visual Studio/VC/Tools/Llvm/x64/bin"
    "C:/Program Files (x86)/Windows Kits/10/bin/$ENV{WindowsSDKVersion}/x64"
    "C:/Program Files (x86)/Windows Kits/10/bin/10.0.22621.0/x64"
    "$ENV{ProgramFiles}/Microsoft DirectX SDK (June 2010)/Utilities/bin/x64"
)
if(DXC_EXECUTABLE)
    message(STATUS "DXC found at: ${DXC_EXECUTABLE}")
else()
    message(FATAL_ERROR "DXC (DirectX Shader Compiler) not found. Please install it.")
endif()

set(SHADER_SOURCES
    raygen.hlsl
    closesthit.hlsl
    miss.hlsl
)

set(SHADER_OUTPUTS)
foreach(SHADER ${SHADER_SOURCES})
    get_filename_component(SHADER_NAME ${SHADER} NAME_WE)
    set(OUTPUT_FILE ${CMAKE_BINARY_DIR}/${SHADER_NAME}.cso)
    add_custom_command(
        OUTPUT ${OUTPUT_FILE}
        COMMAND ${DXC_EXECUTABLE} -T lib_6_3 -Fo ${OUTPUT_FILE} ${CMAKE_SOURCE_DIR}/${SHADER}
        DEPENDS ${SHADER}
        COMMENT "Compiling ${SHADER}"
    )
    list(APPEND SHADER_OUTPUTS ${OUTPUT_FILE})
endforeach()

add_custom_target(shaders DEPENDS ${SHADER_OUTPUTS})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(expert_geometry_3d
    main.cpp
    geometry3d.cpp
    dx12_raytracing.cpp
)

set_property(SOURCE main.cpp PROPERTY SKIP_AUTOMOC OFF)

target_include_directories(expert_geometry_3d PRIVATE . "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/include")
link_directories("${WINDOWS_SDK_DIR}Lib/$ENV{WindowsSDKVersion}um/x64")

target_link_libraries(expert_geometry_3d
    d3d12.lib
    dxgi.lib
    dxguid.lib
    d3dcompiler.lib
    dxcompiler.lib
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)

add_dependencies(expert_geometry_3d shaders)