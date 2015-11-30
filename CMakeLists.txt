cmake_minimum_required(VERSION 2.6)
project(PictureIt)

set(GENERIC_LIB_VERSION "0.0.0")
set(GENERIC_LIB_SOVERSION "0")

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
enable_language(CXX)

# Find and enable C++11
find_package(CXX11 REQUIRED)

# Find and include OpenGL
find_package(OpenGL REQUIRED)
list(APPEND INCLUDES ${OpenGL_INCLUDE_DIRS})
list(APPEND DEPLIBS ${OPENGL_LIBRARIES})
add_definitions(${OpenGL_DEFINITIONS})

list(APPEND PI_INCLUDES ${PROJECT_SOURCE_DIR}/src)
list(APPEND PI_INCLUDES ${PROJECT_SOURCE_DIR}/src/effects)
list(APPEND PI_INCLUDES ${PROJECT_SOURCE_DIR}/src/contrib)

if(WIN32)
    list(APPEND PI_INCLUDES ${PROJECT_SOURCE_DIR}/src/contrib/win32)
endif()

include_directories(${INCLUDES})
include_directories(${PI_INCLUDES})
include_directories(${CMAKE_INCLUDE_PATH})

# Add all sources
file(GLOB_RECURSE PI_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE PI_HEADERS ${PROJECT_SOURCE_DIR}/src/*.h)

# Configure PictureItConfig.cmake.in
set(PICTUREIT_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib)
set(PICTUREIT_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include/pictureit)
configure_file(PictureItConfig.cmake.in "${PROJECT_BINARY_DIR}/PictureItConfig.cmake" @ONLY)

# Enable pic
set(CMAKE_POSITION_INDEPENDENT_CODE 1)

add_library(PictureIt STATIC ${PI_SOURCES} ${PI_HEADERS})
set_target_properties(PictureIt PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(PictureIt PROPERTIES OUTPUT_NAME PictureIt)
set_target_properties(PictureIt PROPERTIES
        COMPILE_DEFINITONS "PICTUREIT_EXPORT"
        VERSION "${GENERIC_LIB_VERSION}"
        SOVERSION "${GENERIC_LIB_SOVERSION}")

install(TARGETS PictureIt DESTINATION ${PICTUREIT_LIB_DIR})
install(FILES ${PI_HEADERS} DESTINATION ${PICTUREIT_INCLUDE_DIRS})
install(FILES "${PROJECT_BINARY_DIR}/PictureItConfig.cmake" DESTINATION "${PICTUREIT_LIB_DIR}/pictureit")
