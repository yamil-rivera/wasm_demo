cmake_minimum_required(VERSION 3.6)

project(wasm_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT WASM_DEMO_PLATFORM)
    string(TOLOWER ${CMAKE_SYSTEM_NAME} WASM_DEMO_PLATFORM)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

include(${CMAKE_SOURCE_DIR}/cmake/${WASM_DEMO_PLATFORM}/platform.cmake
    OPTIONAL
    )

include(${CMAKE_SOURCE_DIR}/cmake/${WASM_DEMO_PLATFORM}/custom_build_configs.cmake
    OPTIONAL
    )

find_package(Threads REQUIRED)

set(WASM_DEMO_EXTERNALS ${CMAKE_SOURCE_DIR}/src/externals)
set(WASM_DEMO_LIBS ${CMAKE_SOURCE_DIR}/src/libs)

include(${CMAKE_SOURCE_DIR}/cmake/${WASM_DEMO_PLATFORM}/openssl.cmake
    OPTIONAL
    )

set(BOOST_ENABLE_FILESYSTEM ON)
set(BOOST_ENABLE_PROGRAM_OPTIONS ON)

set(BOOST_VERSION 1_68_0)
add_subdirectory(${WASM_DEMO_EXTERNALS}/boost_${BOOST_VERSION})

add_subdirectory(${WASM_DEMO_LIBS}/drm_utils)
