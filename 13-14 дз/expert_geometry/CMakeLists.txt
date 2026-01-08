cmake_minimum_required(VERSION 3.10)
project(ExpertGeometry)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(expert_geometry
    main.cpp
    geometry3d.cpp
)

target_include_directories(expert_geometry PRIVATE .)
