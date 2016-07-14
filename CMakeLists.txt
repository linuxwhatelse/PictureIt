cmake_minimum_required(VERSION 3.0)

project(PictureIt LANGUAGES CXX VERSION 0.0.1)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


include(cmake/UseMultiArch.cmake)


if(NOT WIN32)
    set(CMAKE_POSITION_INDEPENDENT_CODE 1)
endif()


# Make sure we use C++11
find_package(CXX11 REQUIRED)


# Find and include OpenGL
find_package(OpenGL REQUIRED)
add_definitions(${OpenGL_DEFINITIONS})
include_directories(${OpenGL_INCLUDE_DIRS})
list(APPEND DEPLIBS ${OPENGL_LIBRARIES})


# Set PictureIt source and header files
set(SOURCES src/pictureit.cpp
            src/spectrum.cpp
            src/utils.cpp
            src/effects/effects.cpp
            src/effects/crossfade.cpp
            src/effects/slide.cpp)

set(HEADERS include/PictureIt/pictureit.h
            include/PictureIt/spectrum.h
            include/PictureIt/utils.h
            include/PictureIt/effects/effects.h
            include/PictureIt/effects/crossfade.h
            include/PictureIt/effects/slide.h)


# Create PictureIt static library.
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})


# Define PictureIt namespace, which is very useful for exporting the PictureIt target.
# ------------------------------------------------------------------------------------------------
# The alias target can be used within the project and ensures with its :: name that 
# target_link_libraries fails already at configure time when the target is not here. 
# Without that, cmake would just assume that it's a system lib and just adds -llib to the 
# linker flag, which would only show errors at link time.
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})


# set PictureIt target include directories
# ------------------------------------------------------------------------------------------------
# PRIVATE, PUBLIC, INTERFACE is best explained here:
# https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html
# PRIVATE   = use it for compiling/linking this library
# INTERFACE = propagate it to users of the library, but don't use it for this library
# PUBLIC    = both
#
# Rule of thumb for target_include_directories(...) is:
# - If you use the dependecy only in your cpp files or private headers --> PRIVATE
# - If you use it in public headers -> INTERFACE
# - If both: PUBLIC

set(INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include/PictureIt
             ${CMAKE_CURRENT_SOURCE_DIR}/deps)
if(WIN32)
    list(APPEND INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/deps/win32)
endif()

target_include_directories(${PROJECT_NAME}
                           PRIVATE   ${INCLUDES}
                           INTERFACE $<INSTALL_INTERFACE:include>
                                     $<INSTALL_INTERFACE:include/PictureIt>)


# Install PictureIt header files.
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include DESTINATION ${CMAKE_INSTALL_PREFIX})

# Export PictureIt target.
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)


# Link all required libraries like OpenGL etc.
target_link_libraries(${PROJECT_NAME} ${DEPLIBS})


# Include configure helper module.
include(CMakePackageConfigHelpers)


# Set PictureIt configure files install path.
set(CMAKE_INSTALL_CMAKECONFIGDIR
    ${CMAKE_INSTALL_LIBDIR_NOARCH}/cmake/${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})


# Generate and install basic PictureIt version file, which can be used by find_package(...).
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
                                 COMPATIBILITY SameMajorVersion)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR})


# Generate and install PictureIt configure file that is used by find_package(...).
# This file makes an find module obsolute, if your project
# sets CMAKE_PREFIX_PATH to the install path of PictureIt.
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
                              INSTALL_DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR})


# Export all targets that are included in the namespace PictureIt.
# Furthermore this will generate the necessary code for PictureIt 
# import as a CMake target for free.
install(EXPORT ${PROJECT_NAME} FILE ${PROJECT_NAME}-targets.cmake NAMESPACE ${CMAKE_PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR})
