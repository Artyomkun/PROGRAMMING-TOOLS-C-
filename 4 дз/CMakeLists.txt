cmake_minimum_required(VERSION 3.10)
project(SequenceCalculator)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sequence_calculator 
    main.cpp 
    sequence_calculator.cpp 
    sequence_calculator.h
)

target_include_directories(sequence_calculator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})