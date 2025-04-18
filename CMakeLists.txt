cmake_minimum_required(VERSION 3.30)

project(CppLearn)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wextra -O3")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

# Conan toolchain
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

# Find dependencies
find_package(Eigen3 REQUIRED)
find_package(GTest REQUIRED)

# Set up directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)

# Include directories
include_directories(${INCLUDE_DIR})

# Gather source and header files
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${INCLUDE_DIR}/*.hpp")

# Add executable
add_executable(dpe_main main.cpp ${SOURCES})
target_compile_options(dpe_main PRIVATE $<$<CONFIG:Debug>:-Wall -Wextra -g -O0> $<$<CONFIG:Release>:-Wall -Wextra -O3>)
# Link dependencies
target_link_libraries(dpe_main PRIVATE Eigen3::Eigen ${CONAN_LIBS})

# Set up tests
if(EXISTS ${TEST_DIR})
    enable_testing()
    file(GLOB_RECURSE TEST_SOURCES "${TEST_DIR}/*.cpp")

    foreach(test_file ${TEST_SOURCES})
        get_filename_component(test_name ${test_file} NAME_WE)
        add_executable(${test_name} ${test_file} ${SOURCES})
        target_include_directories(${test_name} PRIVATE ${INCLUDE_DIR})
        target_compile_options(${test_name} PRIVATE 
            $<$<CONFIG:Debug>:-Wall -Wextra -g -O0> 
            $<$<CONFIG:Release>:-Wall -Wextra -O3>
        )
        target_link_libraries(${test_name} PRIVATE Eigen3::Eigen GTest::gtest GTest::gtest_main ${CONAN_LIBS})
        add_test(NAME ${test_name} COMMAND ${test_name} --gtest_color=yes --gtest_print_time)
    endforeach()
endif()
