cmake_minimum_required(VERSION 2.6)
project(PictureIt)

enable_language(CXX)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

# Find and include OpenGL
find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    list(APPEND INCLUDES ${OpenGL_INCLUDE_DIRS})
    list(APPEND DEPLIBS ${OPENGL_LIBRARIES})
    add_definitions(${OpenGL_DEFINITIONS})
else(OPENGL_FOUND)
    message(FATAL_ERROR "OpenGL not found!")
endif(OPENGL_FOUND)


list(APPEND INCLUDES ${PROJECT_SOURCE_DIR}/src)
list(APPEND INCLUDES ${PROJECT_SOURCE_DIR}/src/effects)
list(APPEND INCLUDES ${PROJECT_SOURCE_DIR}/src/contrib)

if(WIN32)
    list(APPEND INCLUDES ${PROJECT_SOURCE_DIR}/src/contrib/win32)
endif(WIN32)

include_directories(${INCLUDES})
include_directories(${CMAKE_INCLUDE_PATH})

# Add all sources
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

set(CONF_INCLUDE_DIRS "${INCLUDES}")
configure_file(PictureItConfig.cmake.in "${PROJECT_BINARY_DIR}/PictureItConfig.cmake" @ONLY)

# Enable pic
set(CMAKE_POSITION_INDEPENDENT_CODE 1)

add_library(PictureIt STATIC ${SOURCES})

set_property(TARGET PictureIt PROPERTY CXX_STANDARD 11)
