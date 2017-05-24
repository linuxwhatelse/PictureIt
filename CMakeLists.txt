cmake_minimum_required(VERSION 3.1)

project(PictureIt LANGUAGES CXX VERSION 0.0.1)
set(CMAKE_BUILD_TYPE Debug)

if(UNIX)
    add_definitions(-DTARGET_LINUX)
else()
    message(FATAL_ERROR "This plattform is NOT supported!")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


include(GNUInstallDirs)


set(CMAKE_POSITION_INDEPENDENT_CODE 1)


# Make sure we use C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Include GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
list(APPEND DEPLIBS ${GLEW_LIBRARIES})


# Include GLM
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindGLM.cmake)
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})
add_definitions(${GLM_DEFINITIONS})


# Include FFTW
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindFFTW.cmake)
find_package(FFTW REQUIRED)
include_directories(${FFTW_INCLUDES})
list(APPEND DEPLIBS ${FFTW_LIBRARIES})


# Set PictureIt source and header files
set(SOURCES src/pictureit.cpp
            src/utils.cpp
            src/fft.cpp
            src/buffer.cpp
            src/program.cpp

            src/texture/texture.cpp
            src/transitions/crossfade.cpp
            src/transitions/slide.cpp

            src/spectrum/spectrum.cpp
)

set(HEADERS include/PictureIt/pictureit.hpp
            include/PictureIt/utils.hpp
            include/PictureIt/fft.hpp
            include/PictureIt/buffer.hpp
            include/PictureIt/program.hpp

            include/PictureIt/config.hpp
            include/PictureIt/gl_utils.hpp
            include/PictureIt/input.hpp

            include/PictureIt/texture/texture.hpp

            include/PictureIt/transitions/factory.hpp
            include/PictureIt/transitions/itransition.hpp
            include/PictureIt/transitions/crossfade.hpp
            include/PictureIt/transitions/slide.hpp

            include/PictureIt/spectrum/spectrum.hpp

)

set(SHADERS src/spectrum/shader/bar.geom
            src/spectrum/shader/bar_pre.vert
            src/spectrum/shader/bar.vert
            src/spectrum/shader/simple.frag
)


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

set(PictureIt_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(INCLUDES ${PictureIt_INCLUDE_DIR}/PictureIt
             ${CMAKE_CURRENT_SOURCE_DIR}/deps)


target_include_directories(${PROJECT_NAME}
                           PRIVATE ${INCLUDES}
                           INTERFACE
                               $<BUILD_INTERFACE:${PictureIt_INCLUDE_DIR}>
                               $<BUILD_INTERFACE:${PictureIt_INCLUDE_DIR}/PictureIt>
                               $<INSTALL_INTERFACE:include>
                               $<INSTALL_INTERFACE:include>/PictureIt)


# Install PictureIt header files.
install(DIRECTORY ${PictureIt_INCLUDE_DIR} DESTINATION ${CMAKE_INSTALL_PREFIX})

# Export PictureIt target.
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)


# Link all required libraries like OpenGL etc.
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEPLIBS})


# Include configure helper module.
include(CMakePackageConfigHelpers)


# Set PictureIt configure files install path.
set(CMAKE_INSTALL_CMAKECONFIGDIR
    ${CMAKE_INSTALL_FULL_LIBDIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})


# Generate and install basic PictureIt version file, which can be used by find_package(...).
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
                                 COMPATIBILITY SameMajorVersion)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR})


# Generate and install PictureIt configure file that is used by find_package(...).
# This file makes an find module obsolute, if your project
# sets CMAKE_PREFIX_PATH to the install path of PictureIt.
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR}
    PATH_VARS PictureIt_INCLUDE_DIR)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR})


# Export all targets that are included in the namespace PictureIt.
# Furthermore this will generate the necessary code for PictureIt
# import as a CMake target for free.
install(EXPORT ${PROJECT_NAME} FILE ${PROJECT_NAME}-targets.cmake NAMESPACE ${CMAKE_PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR})
