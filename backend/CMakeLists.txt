cmake_minimum_required(VERSION 3.30)

project(DerivativesPricingEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

# Find dependencies
find_package(Eigen3 REQUIRED)
find_package(GTest REQUIRED)
find_package(Crow REQUIRED)

# Set up directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)
add_compile_definitions(PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories
include_directories(${INCLUDE_DIR})

# Gather source and header files
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${INCLUDE_DIR}/*.hpp")

# Common warning flags
#set(WARNING_FLAGS -Wall -Wextra)

set(WARNING_FLAGS
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wcast-align
    -Wunused
    -Wconversion
    -Wsign-conversion
    -Wdouble-promotion
    -Wimplicit-fallthrough
)

# Add main executable
add_executable(dpe_main ${CMAKE_SOURCE_DIR}/main.cpp ${SOURCES})
set_target_properties(dpe_main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
target_compile_options(dpe_main PRIVATE
    $<$<CONFIG:Debug>:${WARNING_FLAGS} -g -O0>
    $<$<CONFIG:Release>:${WARNING_FLAGS} -O3>
)
target_link_libraries(dpe_main PRIVATE Eigen3::Eigen Crow::Crow ${CONAN_LIBS})


add_executable(dpe_server ${CMAKE_SOURCE_DIR}/server_main.cpp ${SOURCES})
set_target_properties(dpe_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
target_compile_options(dpe_server PRIVATE
    $<$<CONFIG:Debug>:${WARNING_FLAGS} -g -O0>
    $<$<CONFIG:Release>:${WARNING_FLAGS} -O3>
)
target_link_libraries(dpe_server PRIVATE Eigen3::Eigen Crow::Crow ${CONAN_LIBS})

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set up tests
if(EXISTS ${TEST_DIR})
    enable_testing()
    file(GLOB_RECURSE TEST_SOURCES "${TEST_DIR}/*.cpp")

    foreach(test_file ${TEST_SOURCES})
        get_filename_component(test_name ${test_file} NAME_WE)
        add_executable(${test_name} ${test_file} ${SOURCES})
        target_include_directories(${test_name} PRIVATE ${INCLUDE_DIR})
        target_compile_options(${test_name} PRIVATE 
            $<$<CONFIG:Debug>:${WARNING_FLAGS} -g -O0> 
            $<$<CONFIG:Release>:${WARNING_FLAGS} -O3>
        )
        target_link_libraries(${test_name} PRIVATE Eigen3::Eigen GTest::gtest GTest::gtest_main ${CONAN_LIBS})
        add_test(NAME ${test_name} COMMAND ${test_name} --gtest_color=yes --gtest_print_time)
    endforeach()
endif()
