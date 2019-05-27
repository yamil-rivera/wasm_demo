cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_SOURCE_DIR}/cmake/${WASM_DEMO_PLATFORM}/boost.cmake)

set(BOOST_CHRONO_SRC
        libs/chrono/src/chrono.cpp
        libs/chrono/src/process_cpu_clocks.cpp
        libs/chrono/src/thread_clock.cpp
        )

set(BOOST_CONTEXT_SRC
        # libs/context/src/dummy.cpp
        libs/context/src/execution_context.cpp
        # libs/context/src/unsupported.cpp
        # libs/context/src/untested.cpp
        ${BOOST_CONTEXT_PLATFORM_FILES}
        )

file(GLOB_RECURSE BOOST_DATE_TIME_SRC
        libs/date_time/src/gregorian/*.cpp
        )

file(GLOB BOOST_FIBER_SRC
        libs/fiber/src/*.cpp
        libs/fiber/src/algo/*.cpp
        )

file(GLOB_RECURSE BOOST_REGEX_SRC
        libs/regex/src/*.cpp
        )

if (BOOST_ENABLE_FILESYSTEM)
        file(GLOB_RECURSE BOOST_FILE_SYSTEM_SRC
                libs/filesystem/src/*.cpp
                )
else ()
        set(BOOST_FILE_SYSTEM_SRC)
endif ()

if (BOOST_ENABLE_PROGRAM_OPTIONS)
        file(GLOB_RECURSE BOOST_PROGRAM_OPTIONS_SRC
                libs/program_options/src/*.cpp
                )
else ()
        set(BOOST_PROGRAM_OPTIONS_SRC)
endif ()

set(BOOST_SYSTEM_SRC
        libs/system/src/error_code.cpp
        )

add_library(boost
        STATIC
        ${BOOST_CHRONO_SRC}
        ${BOOST_CONTEXT_SRC}
        ${BOOST_DATE_TIME_SRC}
        ${BOOST_FIBER_SRC}
        ${BOOST_REGEX_SRC}
        ${BOOST_SYSTEM_SRC}
        ${BOOST_FILE_SYSTEM_SRC}
        ${BOOST_PROGRAM_OPTIONS_SRC}
        )

target_include_directories(boost
        PUBLIC
        .
        )

target_compile_definitions(boost
        PUBLIC
        BOOST_ALL_NO_LIB
        BOOST_CONTEXT_EXPORT
        BOOST_ASIO_HAS_STD_STRING_VIEW
        #BOOST_All_STATIC_LINK
        )

target_compile_options(boost
        PUBLIC
        ${BOOST_PLATFORM_COMPILER_FLAGS}
        )
