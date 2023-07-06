cmake_minimum_required(VERSION 3.2)
project(cgss)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,--enable-stdcall-fixup,--add-stdcall-alias -static-libgcc -static")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--enable-stdcall-fixup,--add-stdcall-alias -static-libgcc -static-libstdc++ -static")
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

set(LIBCGSS_INCLUDE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/")

file(GLOB_RECURSE LIBCGSS_SOURCE_FILES "src/**/*.cpp" "src/**/*.h")

set(LIBCGSS_DEF_FILE src/cgss.def)
set(LIBCGSS_SOURCE_FILES ${LIBCGSS_SOURCE_FILES})

add_library(cgss-static STATIC ${LIBCGSS_SOURCE_FILES})
target_include_directories(cgss-static PUBLIC ${LIBCGSS_INCLUDE_DIRECTORY})
set_target_properties(cgss-static PROPERTIES OUTPUT_NAME cgss)

add_library(cgss-shared SHARED ${LIBCGSS_SOURCE_FILES})
target_include_directories(cgss-shared PUBLIC ${LIBCGSS_INCLUDE_DIRECTORY})
set_target_properties(cgss-shared PROPERTIES OUTPUT_NAME cgss)


if(UNIX AND NOT (CYGWIN OR MINGW))
    target_link_libraries(cgss-static dl)
    target_link_libraries(cgss-shared dl)
endif()

if(WIN32)
    set_target_properties(cgss-static PROPERTIES LINK_DEF_FILE_FLAG ${LIBCGSS_DEF_FILE})
    set_target_properties(cgss-shared PROPERTIES LINK_DEF_FILE_FLAG ${LIBCGSS_DEF_FILE})
endif()

install(
    TARGETS                   cgss-static cgss-shared
    ARCHIVE DESTINATION       lib
    LIBRARY DESTINATION       lib
    RUNTIME DESTINATION       bin
)

install(
    DIRECTORY         ${LIBCGSS_INCLUDE_DIRECTORY}
    DESTINATION       include
)
