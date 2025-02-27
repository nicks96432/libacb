cmake_minimum_required(VERSION 3.25)
project(
    acb
    LANGUAGES CXX
    VERSION 0.4.0
)

option(LIBACB_BUILD_STATIC_LIBS "Build the static library" ON)
option(LIBACB_BUILD_SHARED_LIBS "Build the shared library" ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_CLANG_TIDY clang-tidy)
endif()

find_program(SCCACHE_PATH sccache)
if(SCCACHE_PATH)
    message(STATUS "Using sccache at ${SCCACHE_PATH}")
    set(CMAKE_C_COMPILER_LAUNCHER ${SCCACHE_PATH})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${SCCACHE_PATH})
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT Embedded)
    cmake_policy(SET CMP0141 NEW)
endif()

if(${CMAKE_SIZEOF_VOID_P} EQUAL 4)
    set(TARGET_ARCH x86)
elseif(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    set(TARGET_ARCH x64)
else()
    message(FATAL_ERROR "Unknown pointer size: ${CMAKE_SIZEOF_VOID_P}")
endif()

if(CYGWIN OR MINGW)
    # http://stackoverflow.com/questions/12921911/mingw-libgcc-s-sjlj-1-dll-is-missing
    # However, these options will only work if putting them here, not in 'compiler is gnu c' section.
    add_compile_options(
        -Wl,--enable-stdcall-fixup,--add-stdcall-alias -static-libgcc -static
        $<$<COMPILE_LANGUAGE:CXX>:-static-libstdc++>
    )
    if(CYGWIN)
        # Force using MinGW even though compiling via Cygwin (please install the packages)
        if(${TARGET_ARCH} STREQUAL "x86")
            set(CMAKE_C_COMPILER /usr/bin/i686-w64-mingw32-gcc)
            set(CMAKE_CXX_COMPILER /usr/bin/i686-w64-mingw32-g++)
        elseif(${TARGET_ARCH} STREQUAL "x64")
            set(CMAKE_C_COMPILER /usr/bin/x86_64-w64-mingw32-gcc)
            set(CMAKE_CXX_COMPILER /usr/bin/x86_64-w64-mingw32-g++)
        else()
            message(FATAL_ERROR "Unrecognized target architecture '${TARGET_ARCH}'.")
        endif()
    endif()
    # http://stackoverflow.com/questions/31038963/how-do-you-rename-a-library-filename-in-cmake
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif()

set(is_msvc "$<CXX_COMPILER_ID:MSVC>")
set(debug "$<CONFIG:Debug>")
set(debuginfo "$<CONFIG:Debug,RelWithDebInfo>")

set(warning_flag_msvc /W4 /WX /EHsc)
set(warning_flag_gnu -Wall -Wextra -Wpedantic -Werror -fdiagnostics-color=always)

set(debug_flag_msvc /DEBUG:FULL)
set(debug_flag_gnu -g -ggdb)

add_compile_options(
    "$<IF:${is_msvc},${warning_flag_msvc},${warning_flag_gnu}>"
    "$<${debuginfo}:$<IF:${is_msvc},${debug_flag_msvc},${debug_flag_gnu}>>"
)
add_link_options(
    "$<$<NOT:${is_msvc}>:$<$<NOT:${debug}>:-z;now;>>"
)

configure_file(include/acb_env.h.in include/acb_env.h @ONLY)

file(GLOB_RECURSE LIBACB_SOURCE_FILES "src/**/*.cpp" "src/**/*.h")

add_library(${PROJECT_NAME}-objects OBJECT ${LIBACB_SOURCE_FILES})
set(LIBACB_INCLUDE_DIRECTORY ${CMAKE_SOURCE_DIR}/include/ ${CMAKE_BINARY_DIR}/include/)

target_include_directories(${PROJECT_NAME}-objects PUBLIC ${LIBACB_INCLUDE_DIRECTORY})
set_target_properties(${PROJECT_NAME}-objects PROPERTIES POSITION_INDEPENDENT_CODE 1)

if(UNIX AND NOT (CYGWIN OR MINGW))
    target_link_libraries(acb-objects dl)
endif()

if(LIBACB_BUILD_STATIC_LIBS)
    add_library(${PROJECT_NAME}-static STATIC $<TARGET_OBJECTS:${PROJECT_NAME}-objects>)
    set_target_properties(${PROJECT_NAME}-static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

    install(
        TARGETS                   acb-static
        ARCHIVE DESTINATION       lib
        LIBRARY DESTINATION       lib
        RUNTIME DESTINATION       bin
    )
endif()

if(LIBACB_BUILD_SHARED_LIBS)
    add_library(${PROJECT_NAME}-shared SHARED $<TARGET_OBJECTS:${PROJECT_NAME}-objects>)
    set_target_properties(${PROJECT_NAME}-shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

    install(
        TARGETS                   acb-shared
        ARCHIVE DESTINATION       lib
        LIBRARY DESTINATION       lib
        RUNTIME DESTINATION       lib
    )
endif()

install(
    DIRECTORY         ${LIBACB_INCLUDE_DIRECTORY}
    DESTINATION       include
    FILES_MATCHING
    PATTERN           "*.h"
)
