project(wasm_demo_utils)

include(${CMAKE_SOURCE_DIR}/cmake/${WASM_DEMO_PLATFORM}/openssl.cmake
    OPTIONAL
    )

set(WASM_DEMO_UTILS_HEADERS
        include/drm_utils.h
        )

set(WASM_DEMO_UTILS_SOURCE
        src/crypto_utils.cpp
       )

add_library(wasm_demo_utils
        STATIC
        ${WASM_DEMO_UTILS_HEADERS}
        ${WASM_DEMO_UTILS_SOURCE}
        )

target_include_directories(wasm_demo_utils
        PUBLIC
        ${WASM_DEMO_EXTERNALS}/boost_${BOOST_VERSION}
        ${WASM_DEMO_EXTERNALS}/spdlog-${SPDLOG_VERSION}/include
        ${OPENSSL_INCLUDE_DIR}
        include
        PRIVATE
        src/
        )

target_link_libraries(wasm_demo_utils
        boost
        ${OPENSSL_LIBRARIES}
        )

target_compile_definitions(wasm_demo_utils
        PUBLIC
        ${PLATFORM_DEFINES}
        )
