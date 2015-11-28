cmake_minimum_required(VERSION 2.6)
project(PictureIt)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
enable_language(CXX)

# Find and enable C++11
find_package(CXX11 REQUIRED)

# Find and include OpenGL
find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    list(APPEND INCLUDES ${OpenGL_INCLUDE_DIRS})
    list(APPEND DEPLIBS ${OPENGL_LIBRARIES})
    add_definitions(${OpenGL_DEFINITIONS})
else()
    message(FATAL_ERROR "OpenGL not found!")
endif()


list(APPEND PI_INCLUDES ${PROJECT_SOURCE_DIR}/src)
list(APPEND PI_INCLUDES ${PROJECT_SOURCE_DIR}/src/effects)
list(APPEND PI_INCLUDES ${PROJECT_SOURCE_DIR}/src/contrib)

if(WIN32)
    list(APPEND INCLUDES ${PROJECT_SOURCE_DIR}/src/contrib/win32)
endif()

include_directories(${INCLUDES})
include_directories(${PI_INCLUDES})
include_directories(${CMAKE_INCLUDE_PATH})

# Add all sources
file(GLOB_RECURSE PI_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Confugre PictureItConfig.cmake.in
set(CONF_INCLUDE_DIRS ${INCLUDES} ${PI_INCLUDES})
configure_file(PictureItConfig.cmake.in "${PROJECT_BINARY_DIR}/PictureItConfig.cmake" @ONLY)

# Enable pic
set(CMAKE_POSITION_INDEPENDENT_CODE 1)

add_library(PictureIt STATIC ${PI_SOURCES})
set_target_properties(PictureIt PROPERTIES LINKER_LANGUAGE CXX)

install(TARGETS PictureIt DESTINATION ${PROJECT_SOURCE_DIR}/lib)
