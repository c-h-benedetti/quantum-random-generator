cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(NaturalRandom VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(RandConfig.h.in RandConfig.h)

# Adds the CPR library
add_subdirectory(cpr)

find_package (Threads)

# add the executable
add_executable(NaturalRandom 
    main.cpp
    DataCollector.hpp
    DataCollector.cpp
    QuantumRandom.hpp
    QuantumRandom.cpp
    QuantumRandom.tpp
    Distribution.hpp
    Distribution.cpp
    UniformDistribution.hpp
    UniformDistribution.cpp
    NormalDistribution.hpp
    NormalDistribution.cpp
    ExponentialDistribution.hpp
    ExponentialDistribution.cpp
)

target_link_libraries(NaturalRandom PUBLIC cpr::cpr Threads::Threads)

# Must be after the "add_executable" statement
# The binary's directory ("build") must be an include directory because the .h containing the version will be generated inside
target_include_directories(NaturalRandom PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/cpr"
                           )
