cmake_minimum_required(VERSION 2.6)
project(PictureIt)

enable_language(CXX)

# Check if the compiler support C++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

# Find and include OpenGL
find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    list(APPEND INCLUDES ${OpenGL_INCLUDE_DIRS})
    list(APPEND DEPLIBS ${OPENGL_LIBRARIES})
    add_definitions(${OpenGL_DEFINITIONS})
else()
    message(FATAL_ERROR "OpenGL not found!")
endif()


list(APPEND INCLUDES ${PROJECT_SOURCE_DIR}/src)
list(APPEND INCLUDES ${PROJECT_SOURCE_DIR}/src/effects)
list(APPEND INCLUDES ${PROJECT_SOURCE_DIR}/src/contrib)

if(WIN32)
    list(APPEND INCLUDES ${PROJECT_SOURCE_DIR}/src/contrib/win32)
endif()

include_directories(${INCLUDES})
include_directories(${CMAKE_INCLUDE_PATH})

# Add all sources
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

set(CONF_INCLUDE_DIRS "${INCLUDES}")
configure_file(PictureItConfig.cmake.in "${PROJECT_BINARY_DIR}/PictureItConfig.cmake" @ONLY)

# Enable pic
set(CMAKE_POSITION_INDEPENDENT_CODE 1)

add_library(PictureIt STATIC ${SOURCES})
