cmake_minimum_required(VERSION 2.6)
project(PictureIt CXX)

set(GENERIC_LIB_VERSION "0.0.0")
set(GENERIC_LIB_SOVERSION "0")

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
include_directories(${CMAKE_INCLUDE_PATH})

# Enable pic
if(NOT WIN32)
    set(CMAKE_POSITION_INDEPENDENT_CODE 1)
endif()

# Find and use C++11
find_package(CXX11 REQUIRED)

# Find and include OpenGL
find_package(OpenGL REQUIRED)
add_definitions(${OpenGL_DEFINITIONS})
include_directories(${OpenGL_INCLUDE_DIRS})

# Find and include asplib
find_package(asplib REQUIRED)
include_directories(${ASPLIB_INCLUDE_DIR} ${ASPLIB_INCLUDE_DIRS})

# Define PictureIt includes
list(APPEND PI_INCLUDES "${PROJECT_SOURCE_DIR}/src")
list(APPEND PI_INCLUDES "${PROJECT_SOURCE_DIR}/deps")

if(WIN32)
    list(APPEND PI_INCLUDES "${PROJECT_SOURCE_DIR}/deps/win32")
endif()

# Include PictureIt direcotires
include_directories(${PI_INCLUDES})

set(PI_SOURCES src/pictureit.cpp
               src/spectrum.cpp
               src/utils.cpp
               src/effects/effects.cpp
               src/effects/crossfade.cpp
               src/effects/slide.cpp)

set(PI_HEADERS src/pictureit.h
               src/spectrum.h
               src/utils.h
               src/effects/effects.h
               src/effects/crossfade.h
               src/effects/slide.h)

# Configure PictureItConfig.cmake.in
set(PICTUREIT_LIBRARY_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(PICTUREIT_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include/PictureIt")
list(APPEND PICTUREIT_INCLUDE_DIRS "${ASPLIB_INCLUDE_DIR}"
                                   "${ASPLIB_INCLUDE_DIRS}")
list(APPEND PICTUREIT_LIBRARIES "${ASPLIB_LIBRARY}"
                                "${ASPLIB_LIBRARIES}")
configure_file(PictureItConfig.cmake.in "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/PictureItConfig.cmake" @ONLY)


add_library(PictureIt STATIC ${PI_SOURCES} ${PI_HEADERS})


set_target_properties(PictureIt PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(PictureIt PROPERTIES OUTPUT_NAME PictureIt)
set_target_properties(PictureIt PROPERTIES
        COMPILE_DEFINITONS "PICTUREIT_EXPORT"
        VERSION "${GENERIC_LIB_VERSION}"
        SOVERSION "${GENERIC_LIB_SOVERSION}")

install(TARGETS PictureIt DESTINATION ${PICTUREIT_LIBRARY_DIR} EXPORT PictureIt)

# Install all header files
foreach ( HEADER ${PI_HEADERS} )
    get_filename_component( DIR ${HEADER} DIRECTORY )
    string(REGEX REPLACE "src" "" DIR ${DIR})
    install( FILES ${HEADER} DESTINATION "${PICTUREIT_INCLUDE_DIR}/${DIR}" )
endforeach()

if(WIN32 AND NOT CYGWIN)
    set(DEF_INSTALL_CMAKE_DIR PictureIt)
else()
    set(DEF_INSTALL_CMAKE_DIR lib/PictureIt)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")
install(FILES "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/PictureItConfig.cmake" DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
