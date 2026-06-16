cmake_minimum_required(VERSION 3.20)

project(
    CopyCatCrusher
    VERSION 1.0
    DESCRIPTION "Advanced Source Code Plagiarism Detection Engine"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(
    GLOB_RECURSE
    PROJECT_SOURCES
    CONFIGURE_DEPENDS
    src/*.cpp
)

add_executable(
    CopyCatCrusher
    ${PROJECT_SOURCES}
)

target_include_directories(
    CopyCatCrusher
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

set(
    LLVM_ROOT
    "C:/Program Files/LLVM"
)

target_include_directories(
    CopyCatCrusher
    PRIVATE
    "${LLVM_ROOT}/include"
)

find_library(
    LIBCLANG_LIBRARY
    NAMES
    libclang
    clang
    PATHS
    "${LLVM_ROOT}/lib"
    REQUIRED
)

target_link_libraries(
    CopyCatCrusher
    PRIVATE
    ${LIBCLANG_LIBRARY}
)

if(MSVC)
    target_compile_options(
        CopyCatCrusher
        PRIVATE
        /W4
    )
else()
    target_compile_options(
        CopyCatCrusher
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

if(EXISTS "${LLVM_ROOT}/bin/libclang.dll")
    add_custom_command(
        TARGET CopyCatCrusher
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LLVM_ROOT}/bin/libclang.dll"
        $<TARGET_FILE_DIR:CopyCatCrusher>
    )
endif()

message(STATUS "Using LLVM from: ${LLVM_ROOT}")
message(STATUS "Using libclang: ${LIBCLANG_LIBRARY}")
